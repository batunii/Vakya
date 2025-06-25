from typing import Optional

TT_AT = "@"
TT_LP = "("
TT_RP = ")"
TT_HH = "#"
TT_EQ = "="
TT_LT = "<"
TT_GT = ">"


class Token:
    def __init__(self, t_type: str, t_val: str | None = None):
        self.t_type = t_type
        self.t_val = t_val

    def __repr__(self) -> str:
        return f"{self.t_type} : {self.t_val}" if self.t_val else f"{self.t_type}"


class Lexer:
    def __init__(self, input_code: str):
        self.code = input_code
        self.next_pos = -1
        self.curr_char = None
        self.tokens = []
        self.last_token: Optional[Token] = None

    def adv(self) -> str | None:
        self.next_pos += 1
        self.curr_char = (
            self.code[self.next_pos] if self.next_pos < len(self.code) else None
        )
        self.last_token = self.tokens[-1] if len(self.tokens) > 0 else None
        return self.curr_char

    def make_tokens(self) -> list:
        while char := self.adv():
            match char:
                case "@":
                    self.tokens.append(Token("TT_AT"))
                case "#":
                    self.tokens.append(Token("TT_HH"))
                case "(":
                    self.tokens.append(Token("FUN-DEC"))
                case _:
                    token_attr = ""
                    while char and (char.isalnum() or char == "_"):
                        token_attr += char
                        char = self.adv()
                    if self.last_token:
                        match self.last_token.t_type:
                            case "TT_AT":
                                self.tokens.append(Token("SYS-DEC", token_attr))
                            case "TT_HH":
                                self.tokens.append(Token("USR-DEC", token_attr))
                            case _:
                                self.tokens.append(Token("TT_ATTR", token_attr))
                    else:
                        self.tokens.append(Token("TT_ATTR", token_attr))

        return self.tokens


class Compiler:
    def __init__(self, prompt_: str | None = ""):
        self.prompt = prompt_

    def compile_tokens(self, token_list: list[Token]):
        for idx, token in enumerate(token_list):
            try:
                if token.t_type == "SYS-DEC" and token.t_val == "do":
                    self.prompt = f"{self.prompt} The user wants you to {token_list[idx+1].t_val} "
                elif (
                    token.t_type == "USR-DEC" and token_list[idx - 2].t_type == "TT_AT"
                ):
                    self.prompt = (
                        f"This is the step 1 named : {token.t_val}. \n {self.prompt}"
                    )
            except IndexError as IE:
                print("Wrong Syntax", IE)
                break


compiler = Compiler()
while True:
    code = input("Vakya >> ")
    lexer = Lexer(code)
    lexer.make_tokens()
    print(lexer.tokens)
    compiler.compile_tokens(lexer.tokens)
    print(compiler.prompt)
