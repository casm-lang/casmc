//
//  Copyright (c) 2014-2017 CASM Organization
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

#include "libpass.h"
#include "libstdhl.h"

#include "libcasm-be.h"
#include "libcasm-fe.h"
#include "libcasm-ir.h"
#include "libcasm-rt.h"
#include "libcasm-tc.h"

#include "libcjel-be.h"
#include "libcjel-ir.h"

/**
    @brief TODO

    TODO
*/

// void z3_example( void );

static const std::string DESCRIPTION
    = "Corinthian Abstract State Machine (CASM) Compiler\n";

int main( int argc, const char* argv[] )
{
    libpass::PassManager pm;
    libstdhl::Logger log( pm.stream() );
    log.setSource(
        libstdhl::make< libstdhl::Log::Source >( argv[ 0 ], DESCRIPTION ) );

    auto flush = [&pm, &argv]() {
        libstdhl::Log::ApplicationFormatter f( argv[ 0 ] );
        libstdhl::Log::OutputStreamSink c( std::cerr, f );
        pm.stream().flush( c );
    };

    std::vector< std::string > files_input;
    std::vector< std::string > files_output;

    libstdhl::Args options( argc, argv, libstdhl::Args::DEFAULT,
        [&log, &files_input]( const char* arg ) {

            if( files_input.size() > 0 )
            {
                log.error( "too many files, input file '" + files_input.front()
                           + "' cannot be combined with file '"
                           + arg
                           + "'" );
                return 1;
            }

            files_input.emplace_back( arg );
            return 0;
        } );

    options.add( 't', "test-case-profile", libstdhl::Args::NONE,
        "display the unique test profile identifier",
        [&options]( const char* ) {

            std::cout << libcasm_tc::Profile::get(
                             libcasm_tc::Profile::COMPILER )
                      << "\n";

            return -1;
        } );

    options.add( 'h', "help", libstdhl::Args::NONE,
        "display usage and synopsis", [&log, &options]( const char* ) {

            log.output( "\n" + DESCRIPTION + "\n" + log.source()->name()
                        + ": usage: [options] <file>\n"
                        + "\n"
                        + "options: \n"
                        + options.usage()
                        + "\n" );

            return -1;
        } );

    options.add( 'v', "version", libstdhl::Args::NONE,
        "display version information", [&log]( const char* ) {

            log.output( "\n" + DESCRIPTION + "\n" + log.source()->name()
                        + ": version: "
                        + VERSION
                        + " [ "
                        + __DATE__
                        + " "
                        + __TIME__
                        + " ]\n"
                        + "\n"
                        + LICENSE );

            return -1;
        } );

    options.add( 'o', 0, libstdhl::Args::REQUIRED, "set output <file> path",
        [&log, &files_output]( const char* arg ) {
            if( files_output.size() > 0 )
            {
                log.error( "too many output names passed" );
                return 1;
            }

            files_output.emplace_back( arg );
            return 0;
        },
        "file" );

    for( auto& p : libpass::PassRegistry::registeredPasses() )
    {
        libpass::PassInfo& pi = *p.second;

        if( pi.argChar() or pi.argString() )
        {
            options.add( pi.argChar(), pi.argString(), libstdhl::Args::NONE,
                pi.description(), pi.argAction() );
        }
    }

    if( auto ret = options.parse( log ) )
    {
        flush();

        if( ret >= 0 )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    if( files_input.size() == 0 )
    {
        log.error( "no input file provided" );
        flush();
        return 2;
    }

    // register all wanted passes
    // and configure their setup hooks if desired

    pm.add< libpass::LoadFilePass >(
        [&files_input]( libpass::LoadFilePass& pass ) {
            pass.setFilename( files_input.front() );

        } );

    // pm.setDefaultPass< TODO >();

    //
    // CASM Front-end
    //

    pm.add< libcasm_fe::SourceToAstPass >();
    pm.add< libcasm_fe::AttributionPass >();
    pm.add< libcasm_fe::SymbolResolverPass >();
    pm.add< libcasm_fe::TypeInferencePass >();
    pm.add< libcasm_fe::AstDumpDotPass >();
    // pm.add< libcasm_fe::AstDumpSourcePass >();
    // pm.add< libcasm_fe::NumericExecutionPass >(
    //     [&flag_dump_updates]( libcasm_fe::NumericExecutionPass& pass ) {
    //         pass.setDumpUpdates( flag_dump_updates );

    //     } );
    // pm.add< libcasm_fe::SymbolicExecutionPass >();

    // pm.add< libcasm_fe::AstToCasmIRPass >();

    //
    // CASM Intermediate Representation
    //

    pm.add< libcasm_ir::ConsistencyCheckPass >();
    pm.add< libcasm_ir::IRDumpDebugPass >();
    pm.add< libcasm_ir::IRDumpDotPass >();
    pm.add< libcasm_ir::IRDumpSourcePass >();
    pm.add< libcasm_ir::BranchEliminationPass >();
    // pm.add< libcasm_ir::ConstantFoldingPass >();

    //
    // CASM Back-end
    //

    pm.add< libcasm_be::CasmIRToCjelIRPass >();

    //
    // CJEL Intermediate Representation
    //

    // TODO: PPA: add passes

    //
    // CJEL Back-end
    //

    pm.add< libcjel_be::CjelIRToC11Pass >();
    pm.add< libcjel_be::CjelIRToVHDLPass >();
    pm.add< libcjel_be::CjelIRToLLPass >();
    // pm.add< libcjel_be::CjelIRTo*Pass >();

    try
    {
        pm.run( flush );
    }
    catch( std::exception& e )
    {
        log.error( e.what() );
        flush();
        return -1;
    }

    flush();
    return 0;
}

// // #include <setjmp.h>
// // #include "z3.h"

// void exitf( const char* message )
// {
//     fprintf( stderr, "BUG: %s.\n", message );
//     exit( 1 );
// }

// void error_handler( Z3_context c, Z3_error_code e )
// {
//     fprintf( stderr, "Error code: %d\n", e );
//     exitf( "incorrect use of Z3" );
// }

// /**
//    \brief Create a logical context.

//    Enable model construction. Other configuration parameters can be passed in
//    the cfg variable.

//    Also enable tracing to stderr and register custom error handler.
// */
// Z3_context mk_context_custom( Z3_config cfg, Z3_error_handler err )
// {
//     Z3_context ctx;

//     Z3_set_param_value( cfg, "model", "true" );
//     ctx = Z3_mk_context( cfg );
//     Z3_set_error_handler( ctx, err );

//     return ctx;
// }

// Z3_context mk_context()
// {
//     Z3_config cfg;
//     Z3_context ctx;
//     cfg = Z3_mk_config();
//     ctx = mk_context_custom( cfg, error_handler );
//     Z3_del_config( cfg );
//     return ctx;
// }

// /**
//    \brief Create a variable using the given name and type.
// */
// Z3_ast mk_var( Z3_context ctx, const char* name, Z3_sort ty )
// {
//     Z3_symbol s = Z3_mk_string_symbol( ctx, name );
//     return Z3_mk_const( ctx, s, ty );
// }

// /**
//    \brief Create a boolean variable using the given name.
// */
// Z3_ast mk_bool_var( Z3_context ctx, const char* name )
// {
//     Z3_sort ty = Z3_mk_bool_sort( ctx );
//     return mk_var( ctx, name, ty );
// }

// /**
//    \brief Create an integer variable using the given name.
// */
// Z3_ast mk_int_var( Z3_context ctx, const char* name )
// {
//     Z3_sort ty = Z3_mk_int_sort( ctx );
//     return mk_var( ctx, name, ty );
// }

// void z3_example( void )
// {
//     return;
//     // std::cout << "substitute example\n";
//     // context c;
//     // expr x(c);
//     // x = c.int_const("x");
//     // expr f(c);
//     // f = (x == 2) || (x == 1);
//     // std::cout << f << std::endl;

//     // expr two(c), three(c);
//     // two   = c.int_val(2);
//     // three = c.int_val(3);
//     // Z3_ast from[] = { two };
//     // Z3_ast to[]   = { three };
//     // expr new_f(c);
//     // new_f = to_expr(c, Z3_substitute(c, f, 1, from, to));

//     // std::cout << new_f << std::endl;

//     Z3_context ctx;
//     Z3_sort int_ty;
//     Z3_ast x0, x1;
//     Z3_ast a, b, gb;
//     Z3_func_decl f;
//     Z3_func_decl g;
//     Z3_ast f01, ff010, r;

//     fprintf( stderr, "\nsubstitute_vars_example\n" );

//     ctx = mk_context();
//     int_ty = Z3_mk_int_sort( ctx );
//     x0 = Z3_mk_bound( ctx, 0, int_ty );
//     x1 = Z3_mk_bound( ctx, 1, int_ty );
//     {
//         Z3_sort f_domain[ 2 ] = { int_ty, int_ty };
//         f = Z3_mk_func_decl(
//             ctx, Z3_mk_string_symbol( ctx, "f" ), 2, f_domain, int_ty );
//     }
//     g = Z3_mk_func_decl(
//         ctx, Z3_mk_string_symbol( ctx, "g" ), 1, &int_ty, int_ty );
//     {
//         Z3_ast args[ 2 ] = { x0, x1 };
//         f01 = Z3_mk_app( ctx, f, 2, args );
//     }
//     {
//         Z3_ast args[ 2 ] = { f01, x0 };
//         ff010 = Z3_mk_app( ctx, f, 2, args );
//     }
//     a = mk_int_var( ctx, "a" );
//     b = mk_int_var( ctx, "b" );
//     gb = Z3_mk_app( ctx, g, 1, &b );
//     // Replace x0 -> a, x1 -> g(b) in f(f(x0,x1),x0)
//     {
//         Z3_ast to[ 2 ] = { a, gb };
//         r = Z3_substitute_vars( ctx, ff010, 2, to );
//     }
//     // Display r
//     fprintf( stderr, "substitution result: %s\n", Z3_ast_to_string( ctx, r )
//     );
//     Z3_del_context( ctx );
// }

// Std::string fn( file_name );
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
