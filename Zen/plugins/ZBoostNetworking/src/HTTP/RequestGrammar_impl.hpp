//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
//  Tony Richards trichards@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENTERPRISE_APPSERVER_HTTP_REQUEST_PARSER_IMPL_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_HTTP_REQUEST_PARSER_IMPL_HPP_INCLUDED

/// @note Only include this from RequestParser.cpp

#include <boost/bind.hpp>

#include <boost/spirit/include/classic_confix.hpp>
#include <boost/spirit/include/classic_escape_char.hpp>
#include <boost/spirit/include/classic_lists.hpp>
#include <boost/spirit/include/classic_primitives.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace HTTP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

template <typename ScannerT>
RequestGrammar::definition<ScannerT>::definition(struct RequestGrammar const& _self)
{
#ifdef BOOST_SPIRIT_DEBUG
BOOST_SPIRIT_DEBUG_RULE(method_name);
BOOST_SPIRIT_DEBUG_RULE(initial_line);
BOOST_SPIRIT_DEBUG_RULE(host_line);
BOOST_SPIRIT_DEBUG_RULE(host_name);
BOOST_SPIRIT_DEBUG_RULE(port_number);
BOOST_SPIRIT_DEBUG_RULE(generic_header_line);
BOOST_SPIRIT_DEBUG_RULE(header_line);
BOOST_SPIRIT_DEBUG_RULE(header_lines);
BOOST_SPIRIT_DEBUG_RULE(spaces);
BOOST_SPIRIT_DEBUG_RULE(url);
BOOST_SPIRIT_DEBUG_RULE(http_version);
BOOST_SPIRIT_DEBUG_RULE(request_parser);
#endif

    method_name =
        boost::spirit::classic::str_p("GET")             [boost::bind(&RequestParserState::get, &_self.m_parserState, _1, _2)]
        |   boost::spirit::classic::str_p("POST")
        |   boost::spirit::classic::str_p("HEAD")
    ;

    url = 
            !boost::spirit::classic::str_p("http:/")
        >>  *(boost::spirit::classic::anychar_p - spaces)
    ;

    http_version =
            boost::spirit::classic::str_p("HTTP/")
        >>  boost::spirit::classic::int_p
        >>  boost::spirit::classic::str_p(".")
        >>  boost::spirit::classic::int_p
    ;

    initial_line = 
            method_name
        >>  spaces
        >>  url                                 [boost::bind(&RequestParserState::url, &_self.m_parserState, _1, _2)]
        >>  spaces
        >>  http_version
        >>  boost::spirit::classic::eol_p
    ;

    host_name =
        // HACK Do a better job parsing this
        anything_but_colon
    ;

    port_number =
        boost::spirit::classic::int_p
    ;

    host_line = 
            boost::spirit::classic::str_p("Host: ")
        >>  host_name
        >>  boost::spirit::classic::str_p(":")
        >>  port_number
        >>  boost::spirit::classic::eol_p
    ;

    anything_but_colon =
        *(boost::spirit::classic::anychar_p - boost::spirit::classic::str_p(":") - boost::spirit::classic::eol_p)
    ;

    anything_but_crlf =
        *(boost::spirit::classic::anychar_p - boost::spirit::classic::eol_p)
    ;

    spaces =
        *boost::spirit::classic::space_p
    ;

    generic_header_line =
            anything_but_colon
        >>  spaces
        >>  boost::spirit::classic::str_p(":")
        >>  spaces
        >>  anything_but_crlf
        >>  boost::spirit::classic::eol_p
    ;

    header_line =
            host_line
        |   generic_header_line
    ;

    header_lines =
        *header_line
    ;

    request_parser =
            initial_line
        >>  header_lines
        >>  boost::spirit::classic::eol_p                [boost::bind(&RequestParserState::done, &_self.m_parserState, _1, _2)]
    ;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace HTTP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // ZEN_ENTERPRISE_APPSERVER_HTTP_REQUEST_PARSER_IMPL_HPP_INCLUDED
