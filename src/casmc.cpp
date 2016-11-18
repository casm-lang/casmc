//
//  Copyright (c) 2014-2016 CASM Organization https://github.com/casm-lang
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                Florian Hahn
//                https://github.com/casm-lang/casmc
//
//  This file is part of casmc.
//
//  casmc is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  casmc is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with casmc. If not, see <http://www.gnu.org/licenses/>.
//

#include "license.h"
#include "version.h"

#include "cpp/Args.h"
#include "cpp/Default.h"

#include "libcasm-be.all.h"
#include "libcasm-fe.all.h"
#include "libcasm-ir.all.h"
#include "libcasm-tc.h"
#include "libnovel.all.h"
#include "libpass.h"

/**
    @brief TODO

    TODO
*/

void z3_example( void );

int main( int argc, const char* argv[] )
{
    z3_example();

    const char* file_name = 0;
    const char* output_name = 0;

    Args options(
        argc, argv, Args::DEFAULT, [&file_name, &options]( const char* arg ) {
            static int cnt = 0;
            cnt++;

            if( cnt > 1 )
            {
                options.error( 1, "to many file names passed" );
            }

            file_name = arg;
        } );

    options.add( "tc", Args::NONE,
        "Displays the test case unique identifier and exits.",
        [&options, &output_name]( const char* option ) {
            printf( "%s\n",
                libcasm_tc::Profile::get( libcasm_tc::Profile::COMPILER ) );
            exit( 0 );
        } );

    options.add( 'o', 0, Args::REQUIRED, "Place the output into <file>",
        [&options, &output_name]( const char* option ) {
            static int cnt = 0;
            cnt++;

            if( cnt > 1 )
            {
                options.error( 1, "to many output names passed" );
            }

            output_name = option;
        },
        "file" );

#define DESCRIPTION "Corinthian Abstract State Machine (CASM) Compiler\n"

    options.add( 'h', "help", Args::NONE,
        "Display the program usage and synopsis",
        [&options]( const char* option ) {
            fprintf( stderr, DESCRIPTION
                "\n"
                "usage: %s [options] <file>\n"
                "\n"
                "options:\n",
                options.getProgramName() );

            options.usage();

            exit( 0 );
        } );

    options.add( 'v', "version", Args::NONE,
        "Display compiler version information",
        [&options]( const char* option ) {
            fprintf( stderr, DESCRIPTION
                "\n"
                "%s: version: %s [ %s %s ]\n"
                "\n"
                "%s\n",
                options.getProgramName(), VERSION, __DATE__, __TIME__,
                LICENSE );

            exit( 0 );
        } );

    for( auto& p : libpass::PassRegistry::getRegisteredPasses() )
    {
        // PassId    id = p.first;
        libpass::PassInfo& pi = *p.second;

        if( pi.getPassArgChar() == 0 && pi.getPassArgString() == 0 )
        {
            // internal pass, do not register a cmd line flag
            continue;
        }

        options.add( pi.getPassArgChar(), pi.getPassArgString(), Args::NONE,
            pi.getPassDescription(), [&pi]( const char* option ) {
                printf( "add: %s:%s\n", pi.getPassName(), option );
                // add to PassManager the selected pass to run!
            } );
    }

    options.parse();

    if( !file_name )
    {
        options.error( 1, "no input file provided" );
    }

    libpass::PassResult x;
    x.getResults()[ 0 ] = (void*)file_name;
    x.getResults()[ (void*)1 ] = (void*)output_name;

    libcasm_fe::SourceToAstPass src2ast;
    if( !src2ast.run( x ) )
    {
        return -1;
    }

    libcasm_fe::TypeCheckPass ast_type;
    if( !ast_type.run( x ) )
    {
        return -1;
    }

    libcasm_fe::AstDumpPass ast_dump;
    ast_dump.run( x );

    libcasm_ir::AstToCasmIRPass ast2ir;
    ast2ir.run( x );

    // libcasm_be::CasmIRToLLCodePass ir2ll;
    // ir2ll.run( x );

    libcasm_ir::CasmIRDumpPass ir_dump;
    printf( "\n===--- DUMPING CASM IR ---===\n" );
    ir_dump.run( x );

    libcasm_be::CasmIRToNovelPass ir2novel;
    printf( "\n===--- CASM IR to NOVEL ---===\n" );
    ir2novel.run( x );
    libnovel::Module* m
        = (libnovel::Module*)x.getResult< libcasm_be::CasmIRToNovelPass >();

    libnovel::NovelDumpPass novel_dump;
    printf( "\n===--- DUMPING NOVEL ---===\n" );
    novel_dump.run( x );

    libnovel::NovelToC11Pass novel2c11;
    printf( "\n===--- NOVEL to C11 ---===\n" );
    novel2c11.run( x );

    std::string fnc( "obj/" + std::string( m->getName() ) + ".c" );
    std::string cmd( "time clang -Wall -g -O0 " + fnc + " -o " + fnc + ".bin" );
    printf( "'%s'\n", cmd.c_str() );
    system( cmd.c_str() );

    cmd = std::string( "time clang -O1 " + fnc + " -o " + fnc + ".bin.O1" );
    printf( "'%s'\n", cmd.c_str() );
    system( cmd.c_str() );
    cmd = std::string( "time clang -O3 " + fnc + " -o " + fnc + ".bin.O3" );
    printf( "'%s'\n", cmd.c_str() );
    system( cmd.c_str() );

    // libnovel::NovelToLLPass novel2ll;
    // printf( "\n===--- NOVEL to LL ---===\n" );
    // ll.run( x );

    libnovel::NovelToVHDLPass novel2vhdl;
    printf( "\n===--- NOVEL to VHDL ---===\n" );
    novel2vhdl.run( x );

    // transform CASM AST -> IR

    // optimizations (analysis and transformations)

    // interpretation: CASM-based

    // code-generation: CASM IR --+-> LLVM IR -> exec
    //                            |
    //                            +-> LLVM IR (interpretation via lli)
    return 0;
}

#include <setjmp.h>

#include "z3.h"

/**
   \brief exit gracefully in case of error.
*/
void exitf( const char* message )
{
    fprintf( stderr, "BUG: %s.\n", message );
    exit( 1 );
}

/**
   \brief Simpler error handler.
*/
void error_handler( Z3_context c, Z3_error_code e )
{
    printf( "Error code: %d\n", e );
    exitf( "incorrect use of Z3" );
}

/**
   \brief Create a logical context.

   Enable model construction. Other configuration parameters can be passed in
   the cfg variable.

   Also enable tracing to stderr and register custom error handler.
*/
Z3_context mk_context_custom( Z3_config cfg, Z3_error_handler err )
{
    Z3_context ctx;

    Z3_set_param_value( cfg, "model", "true" );
    ctx = Z3_mk_context( cfg );
    Z3_set_error_handler( ctx, err );

    return ctx;
}

Z3_context mk_context()
{
    Z3_config cfg;
    Z3_context ctx;
    cfg = Z3_mk_config();
    ctx = mk_context_custom( cfg, error_handler );
    Z3_del_config( cfg );
    return ctx;
}

/**
   \brief Create a variable using the given name and type.
*/
Z3_ast mk_var( Z3_context ctx, const char* name, Z3_sort ty )
{
    Z3_symbol s = Z3_mk_string_symbol( ctx, name );
    return Z3_mk_const( ctx, s, ty );
}

/**
   \brief Create a boolean variable using the given name.
*/
Z3_ast mk_bool_var( Z3_context ctx, const char* name )
{
    Z3_sort ty = Z3_mk_bool_sort( ctx );
    return mk_var( ctx, name, ty );
}

/**
   \brief Create an integer variable using the given name.
*/
Z3_ast mk_int_var( Z3_context ctx, const char* name )
{
    Z3_sort ty = Z3_mk_int_sort( ctx );
    return mk_var( ctx, name, ty );
}

void z3_example( void )
{
    return;
    // std::cout << "substitute example\n";
    // context c;
    // expr x(c);
    // x = c.int_const("x");
    // expr f(c);
    // f = (x == 2) || (x == 1);
    // std::cout << f << std::endl;

    // expr two(c), three(c);
    // two   = c.int_val(2);
    // three = c.int_val(3);
    // Z3_ast from[] = { two };
    // Z3_ast to[]   = { three };
    // expr new_f(c);
    // new_f = to_expr(c, Z3_substitute(c, f, 1, from, to));

    // std::cout << new_f << std::endl;

    Z3_context ctx;
    Z3_sort int_ty;
    Z3_ast x0, x1;
    Z3_ast a, b, gb;
    Z3_func_decl f;
    Z3_func_decl g;
    Z3_ast f01, ff010, r;

    printf( "\nsubstitute_vars_example\n" );

    ctx = mk_context();
    int_ty = Z3_mk_int_sort( ctx );
    x0 = Z3_mk_bound( ctx, 0, int_ty );
    x1 = Z3_mk_bound( ctx, 1, int_ty );
    {
        Z3_sort f_domain[ 2 ] = { int_ty, int_ty };
        f = Z3_mk_func_decl(
            ctx, Z3_mk_string_symbol( ctx, "f" ), 2, f_domain, int_ty );
    }
    g = Z3_mk_func_decl(
        ctx, Z3_mk_string_symbol( ctx, "g" ), 1, &int_ty, int_ty );
    {
        Z3_ast args[ 2 ] = { x0, x1 };
        f01 = Z3_mk_app( ctx, f, 2, args );
    }
    {
        Z3_ast args[ 2 ] = { f01, x0 };
        ff010 = Z3_mk_app( ctx, f, 2, args );
    }
    a = mk_int_var( ctx, "a" );
    b = mk_int_var( ctx, "b" );
    gb = Z3_mk_app( ctx, g, 1, &b );
    // Replace x0 -> a, x1 -> g(b) in f(f(x0,x1),x0)
    {
        Z3_ast to[ 2 ] = { a, gb };
        r = Z3_substitute_vars( ctx, ff010, 2, to );
    }
    // Display r
    printf( "substitution result: %s\n", Z3_ast_to_string( ctx, r ) );
    Z3_del_context( ctx );
}

// std::string fn( file_name );
// fn += ".dot";

// std::ofstream fd;
// fd.open( fn );
// fd << dump.get_dump();
// fd.close();

// std::string cmd;
// cmd += "dot -Tpdf ";
// cmd += fn;
// cmd += " -o ";
// cmd += fn;
// cmd += ".pdf";

// system( cmd.c_str() );

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//
