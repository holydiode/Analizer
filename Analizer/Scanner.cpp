#include "Scanner.h"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>

#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define IS_LETTER(c)  ((c >= 'a') && (c <= 'z') || (c >= 'A') && (c <= 'Z'))

const char* keyword[7] = { "class", "for", "double", "boolean", "return", "true", "false"};
LexType indexKeyword[7] = {
    LexType::CLASS_KEYWORD,
    LexType::FOR_KEYWORD,
    LexType::STANDART_DATA_TYPE,
    LexType::STANDART_DATA_TYPE,
    LexType::RETURN_KEYWORD,
    LexType::CONSTANT,
    LexType::CONSTANT
};



Cursore Scanner::skip_ignore()
{
    Cursore diff;
    diff.column = 0;
    diff.row = 0;
    while (*cur_char == ' ' || *cur_char == '\n' || *cur_char == '\t'|| *cur_char == '\r') {
        diff.column++;
        if (*cur_char == '\n') {
            diff.row++;
            diff.column = 0;
        }
        cur_char++;
    }

    if (*cur_char == '/' && *(cur_char + 1) == '/') {
        cur_char += 2;
        while (*cur_char != '\n' || *(cur_char + 1) == '\0') {
            cur_char++;
        }
        diff.row++;
        diff.column = 0;
    }
    return diff;
}

LexType Scanner::word_lexem()
{
    char* lex = (char*)malloc(50);
    int i = 0;
    while (IS_DIGIT(*cur_char) || IS_LETTER(*cur_char) || *cur_char == '_')
    {
        lex[i++] = *cur_char;
        cur_char++;
    }
    lex[i] = '\0';
    for (int j = 0; j < 5; j++)
        if (strcmp(lex, keyword[j]) == 0)
            return indexKeyword[j];
    return LexType::NAME;    
}

LexType Scanner::number_lexem()
{
    while (IS_DIGIT(*cur_char))
        cur_char++;
    return LexType::CONSTANT;
}

LexType Scanner::scan()
{
    Cursore line_skiped = skip_ignore();
    while (line_skiped.row || line_skiped.column )
    {
        line_skiped = skip_ignore();
    }

    if (IS_LETTER(*cur_char) || *cur_char == '_')
        return word_lexem();

    if (IS_DIGIT(*cur_char))
        return number_lexem();

    if (*cur_char == '\0')
        return LexType::EOF_TYPE;


    if (*cur_char == '|') {
        cur_char++;
        if(*cur_char == '|') {
            cur_char++;
            return LexType::OR_SIGN;
        }
        return LexType::DISUNC_SIGN;
    }

    if (*cur_char == '&') {
        cur_char++;
        if (*cur_char == '&') {
            cur_char++;
            return LexType::AND_SIGN;
        }
        return LexType::CONUC_SIGN;
    }

    if (*cur_char == '=') {
        cur_char++;
        if (*cur_char == '=') {
            cur_char++;
            return LexType::EQUAL_SIGN;
        }
        return LexType::ASSIGN_SIGN;
    }

    if (*cur_char == '<') {
        cur_char++;
        if (*cur_char == '=') {
            cur_char++;
            return LexType::LESS_OR_EQUAL_SIGN;
        }
        return LexType::LESS_SIGN;
    }

    if (*cur_char == '>') {
        cur_char++;
        if (*cur_char == '=') {
            cur_char++;
            return LexType::MORE_OR_EQUAL_SIGN;
        }
        return LexType::MORE_SIGN;
    }

    if (*cur_char == '!' && *(cur_char + 1) == '=') {
        cur_char += 2;
        return LexType::NOT_EQUAL_SING;
    }

    if (*cur_char == '+') {
        cur_char++;
        return LexType::PLUS_SIGN;
    }

    if (*cur_char == '-') {
        cur_char++;
        return LexType::MINUS_SIGN;
    }

    if (*cur_char == '*') {
        cur_char++;
        return LexType::MULTIPLE_SING;
    }
    
    if (*cur_char == '/') {
        cur_char++;
        return LexType::DIV_SIGN;
    }
    
    if(*cur_char == '%') {
        cur_char++;
        return LexType::MOD_SIGN;
    }    

    if(*cur_char == ';') {
        cur_char++;
        return LexType::SEMY;
    }

    if(*cur_char == '(') {
        cur_char++;
        return LexType::LEFT_PRNT;
    }

    if(*cur_char == ')') {
        cur_char++;
        return LexType::RIGHT_PRNT;
    }

    if(*cur_char == '{') {
        cur_char++;
        return LexType::LEFT_BRK;
    }

    if(*cur_char == '}') {
        cur_char++;
        return LexType::RIGHT_BRK;
    }

    if (*cur_char == '.') {
        cur_char++;
        return LexType::DOT;
    }

    if (*cur_char == ',') {
        cur_char++;
        return LexType::COL;
    }

    return LexType::EOF_TYPE;
}

LexType Scanner::watch()
{
    char* save_pointer = cur_char;
    LexType type = scan();
    cur_char = save_pointer;
    return type;
}

void Scanner::read(const char* filename)
{
    std::ifstream file(filename, std::ios::binary);
    std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});
    file.close();

    text = new char[buffer.size()];
    std::copy(buffer.begin(), buffer.end(), text);

    cur_char = text;
}
