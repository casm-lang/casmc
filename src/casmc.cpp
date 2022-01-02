//
//  Copyright (C) 2014-2022 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//                <https://github.com/casm-lang/casmc/graphs/contributors>
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

#include <casmc/Version>

#include <libcasm-be/libcasm-be>
#include <libcasm-fe/libcasm-fe>
#include <libcasm-ir/libcasm-ir>

#include <libcjel-be/libcjel-be>
#include <libcjel-ir/libcjel-ir>

#include <libpass/libpass>
#include <libstdhl/libstdhl>

/**
    @brief TODO

    TODO
*/

// void z3_example( void );

static const std::string DESCRIPTION = "Corinthian Abstract State Machine (CASM) Compiler\n";
static const std::string PROFILE = "casmc";

int main( int argc, const char* argv[] )
{
    assert( argc > 0 );
    const std::string app_name = argv[ 0 ];

    libpass::PassManager pm;
    libstdhl::Logger log( pm.stream() );
    log.setSource( libstdhl::Memory::make< libstdhl::Log::Source >( app_name, DESCRIPTION ) );

    auto flush = [&pm, &app_name]() {
        libstdhl::Log::ApplicationFormatter f( app_name );
        libstdhl::Log::OutputStreamSink c( std::cerr, f );
        pm.stream().flush( c );
    };

    std::vector< std::string > files;
    std::vector< std::string > files_output;
    u1 flag_dump_updates = false;

    libstdhl::Args options( argc, argv, libstdhl::Args::DEFAULT, [&files, &log]( const char* arg ) {
        if( files.size() > 0 )
        {
            log.error(
                "too many files, input file '" + files.front() +
                "' cannot be combined with file '" + arg + "'" );
            return 1;
        }

        files.emplace_back( arg );
        return 0;
    } );

    options.add(
        't',
        "test-case-profile",
        libstdhl::Args::NONE,
        "display the unique test profile identifier",
        []( const char* ) {
            std::cout << PROFILE << "\n";
            return -1;
        } );

    options.add(
        'h',
        "help",
        libstdhl::Args::NONE,
        "display usage and synopsis",
        [&log, &options]( const char* ) {
            log.output(
                "\n" + DESCRIPTION + "\n" + log.source()->name() + ": usage: [options] <file>\n" +
                "\n" + "options: \n" + options.usage() + "\n" );

            return -1;
        } );

    options.add(
        'v', "version", libstdhl::Args::NONE, "display version information", [&log]( const char* ) {
            log.output(
                "\n" + DESCRIPTION + "\n" + log.source()->name() + ": version: " + casmc::REVTAG +
                " [ " + __DATE__ + " " + __TIME__ + " ]\n" + "\n" + casmc::NOTICE );

            return -1;
        } );

    options.add(
        'o',
        0,
        libstdhl::Args::REQUIRED,
        "set output <file> path",
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

    // add passes to the pass manager to setup command-line options

    pm.add< libcasm_fe::AstDumpDotPass >();

    pm.add< libcjel_be::CjelIRToC11Pass >();
    pm.add< libcjel_be::CjelIRToLLPass >();
    pm.add< libcjel_be::CjelIRToVHDLPass >();

    for( auto id : pm.passes() )
    {
        libpass::PassInfo& pi = libpass::PassRegistry::passInfo( id );

        if( pi.argChar() or pi.argString() )
        {
            options.add(
                pi.argChar(),
                pi.argString(),
                libstdhl::Args::NONE,
                pi.description(),
                pi.argAction() );
        }
    }

    // parse the command-line

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

    if( files.size() == 0 )
    {
        log.error( "no input file provided" );
        flush();
        return 2;
    }

    // set default settings

    libpass::PassResult pr;
    pr.setInput< libpass::LoadFilePass >( files.front() );

    pm.setDefaultResult( pr );
    pm.setDefaultPass< libcjel_be::CjelIRToC11Pass >();

    // set pass-specific configurations

    // run pass pipeline

    if( not pm.run( flush ) )
    {
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
