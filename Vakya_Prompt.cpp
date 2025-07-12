#include "Vakya_Compiler.hpp"
#include "Vakya_Error.hpp"
#include "Vakya_Lexer.hpp"
#include "Vakya_Program.hpp"
#include <iostream>
#include <sstream>
#include <string>
void add_header(std::stringstream &prompt) {
  prompt << "========================================================\n"
         << "TASK BRIEF & EXECUTION POLICY\n"
         << "--------------------------------------------------------\n"
         << "You are a response engine processing structured instructions from "
            "a DSL interpreter.\n"
         << "Your job is to understand, obey, and execute ONLY what the user "
            "has explicitly requested.\n"
         << "No assumptions, generalizations, simplifications, or "
            "extrapolations are allowed.\n"
         << "Everything is defined by priority:\n"
         << "  - MUST (Priority 1): Mandatory instructions. Failure to comply "
            "invalidates the result.\n"
         << "  - SHOULD (Priority 2): Important but not critical. Try to "
            "include them.\n"
         << "  - COULD (Priority 3): Optional. Include only if trivially "
            "available or naturally derivable.\n"
         << "Each section below specifies details about what to do, how to do "
            "it, which sources to use, "
         << "and how to structure the result.\n\n";
}
void add_footer(std::stringstream &prompt) {
  prompt
      << "============================================================\n"
      << "FINAL CHECKLIST & COMPLIANCE REMINDERS\n"
      << "------------------------------------------------------------\n"
      << "Before returning your output, ensure the following:\n"
      << "  ✅ All PRIORITY 1 (MUST) instructions are satisfied.\n"
      << "  ✅ All PRIORITY 2 (SHOULD) instructions are included if feasible.\n"
      << "  ✅ PRIORITY 3 (COULD) parts are included only if they come up "
         "naturally and accurately.\n"
      << "  ⚠️  Absolutely nothing outside the explicit instructions has been "
         "added.\n"
      << "  🧱 Output is structured according to the format specified in the "
         "@fmt section.\n"
      << "  🔒 Strict mode has been respected if enabled.\n"
      << "------------------------------------------------------------\n"
      << "END OF DSL-INTERPRETED PROMPT\n"
      << "============================================================\n";
}
void do_on(std::stringstream &prompt, Program *prgrm) {
  prompt << "============================================================\n"
         << "ACTION\n"
         << "------------------------------------------------------------\n"
         << "The user wants you to: " << prgrm->do_token->action_props + "\n"
         << "On the topic and Target entity: " + prgrm->on_token->action_props +
                "\n";
}

void add_sources(std::stringstream &prompt, Program *prgrm) {
  if (prgrm->src_token) {
    prompt << "============================================================\n"
           << "SOURCES\n"
           << "------------------------------------------------------------\n";
    prompt << "Sources to be used while performing the action, prioritized: \n";
    if (prgrm->src_token->action_props.must) {
      prompt << "These are must, priority 1 sources and should definitly be "
                "included : \n";
      prompt << prgrm->src_token->action_props.must << "\n";
    }
    if (prgrm->src_token->action_props.should) {
      prompt << "These are should, priority 2 sources and should attempted to "
                "included : \n";
      prompt << prgrm->src_token->action_props.should << "\n";
    }
    if (prgrm->src_token->action_props.could) {
      prompt << "These are must, priority 3 sources and optional to be "
                "included if possible "
                "included : \n";
      prompt << prgrm->src_token->action_props.could << "\n";
    }
  }
}
void add_formatting(std::stringstream &prompt, Program *prgrm) {
  if (!prgrm->fmt_token || !prgrm->fmt_token->type)
    return;

  prompt << "============================================================\n"
         << "FORMATTING INSTRUCTIONS\n"
         << "------------------------------------------------------------\n";

  // Output type (e.g., #table, #paragraph, etc.)
  std::string fmt_type = prgrm->fmt_token->type->action_name;
  prompt << "The user expects the final result to be structured using the "
            "following format type:\n";
  prompt << "→ Format type: **" << fmt_type << "**\n\n";

  // Type-specific props
  if (fmt_type.find("table") != std::string::npos) {
    prompt << "This means the output should be in tabular form with the "
              "following columns:\n";
    if (prgrm->fmt_token->type->action_props.must) {
      prompt << "These are MUST HAVE columns (priority 1):\n";
      prompt << prgrm->fmt_token->type->action_props.must << "\n";
    }
    if (prgrm->fmt_token->type->action_props.should) {
      prompt << "These are NICE TO HAVE columns (priority 2):\n";
      prompt << prgrm->fmt_token->type->action_props.should << "\n";
    }
    if (prgrm->fmt_token->type->action_props.could) {
      prompt << "These are OPTIONAL columns (priority 3):\n";
      prompt << prgrm->fmt_token->type->action_props.could << "\n";
    }

  } else if (fmt_type.find("paragraph") != std::string::npos) {
    prompt << "This means the output should be a set of structured paragraphs "
              "with the following content preferences:\n";
    if (prgrm->fmt_token->type->action_props.must) {
      prompt << "These are mandatory elements to be included in every "
                "paragraph (priority 1):\n";
      prompt << prgrm->fmt_token->type->action_props.must << "\n";
    }
    if (prgrm->fmt_token->type->action_props.should) {
      prompt << "These are preferred elements for paragraph content (priority "
                "2):\n";
      prompt << prgrm->fmt_token->type->action_props.should << "\n";
    }
    if (prgrm->fmt_token->type->action_props.could) {
      prompt << "These are optional elements to include in paragraphs if "
                "possible (priority 3):\n";
      prompt << prgrm->fmt_token->type->action_props.could << "\n";
    }

  } else if (fmt_type == "mind_map") {
    prompt << "The user expects a **mind map**, where the central idea is "
              "broken down into branches of related concepts.\n\n";

  } else if (fmt_type == "knowledge_web") {
    prompt << "The user expects a **knowledge web**, where ideas are shown as "
              "interconnected nodes without strict hierarchy.\n\n";

  } else {
    prompt << "Unrecognized format type. Please attempt to interpret based on "
              "fallback to structured text blocks.\n\n";
  }

  // Optional: order and meta
  if (prgrm->fmt_token->order) {
    prompt << "------------------------------------------------------------\n"
           << "ORDERING RULES\n";
    prompt
        << "These conditions define how to sort or group the output content.\n";
    if (prgrm->fmt_token->order->action_props.must) {
      prompt << "These must be strictly followed for ordering (priority 1):\n";
      prompt << prgrm->fmt_token->order->action_props.must << "\n";
    }
    if (prgrm->fmt_token->order->action_props.should) {
      prompt << "These should be followed for better structure (priority 2):\n";
      prompt << prgrm->fmt_token->order->action_props.should << "\n";
    }
    if (prgrm->fmt_token->order->action_props.could) {
      prompt << "These are optional ordering rules (priority 3):\n";
      prompt << prgrm->fmt_token->order->action_props.could << "\n";
    }
  }

  if (prgrm->fmt_token->meta) {
    prompt << "------------------------------------------------------------\n"
           << "FORMAT METADATA\n";
    prompt << "Use the following metadata to further constrain or guide the "
              "output.\n";
    if (prgrm->fmt_token->meta->action_props.must) {
      prompt << "These are critical formatting rules (priority 1):\n";
      prompt << prgrm->fmt_token->meta->action_props.must << "\n";
    }
    if (prgrm->fmt_token->meta->action_props.should) {
      prompt << "These metadata rules are preferred (priority 2):\n";
      prompt << prgrm->fmt_token->meta->action_props.should << "\n";
    }
    if (prgrm->fmt_token->meta->action_props.could) {
      prompt << "These are optional format-level enhancements (priority 3):\n";
      prompt << prgrm->fmt_token->meta->action_props.could << "\n";
    }
  }
}

void add_conditions(std::stringstream &prompt, Program *prgrm) {
  if (prgrm->src_token) {
    prompt << "============================================================\n"
           << "CONDITIONS\n"
           << "------------------------------------------------------------\n";
    prompt
        << "Conditions to be used while filtering and curating the result. \n";
    if (prgrm->cdn_token->action_props.must) {
      prompt << "These are must, priority 1 conditions and should definitly be "
                "followed. More than 1 conditions can be considered AND "
                "conditions here : \n";
      prompt << prgrm->cdn_token->action_props.must << "\n";
    }
    if (prgrm->cdn_token->action_props.should) {
      prompt << "These are should, priority 2 conditions and should be tried "
                "to follow : \n";
      prompt << prgrm->cdn_token->action_props.should << "\n";
    }
    if (prgrm->cdn_token->action_props.could) {
      prompt << "These are could, priority 3 conditions and optional to be "
                "included if possible: \n";
      prompt << prgrm->cdn_token->action_props.could << "\n";
    }
  }
}
void add_strict(std::stringstream &prompt, Program *prgrm) {
  prompt << "============================================================\n"
         << "STRICTNESS POLICY [@strict]\n"
         << "------------------------------------------------------------\n";

  if (prgrm->strict) {
    prompt << "The STRICT flag has been enabled.\n"
           << "This means the response must be generated EXACTLY as per "
              "instructions. "
           << "The following are explicitly prohibited:\n"
           << "  - ❌ No summaries, overviews, or generic conclusions.\n"
           << "  - ❌ No extra explanations or helpful additions.\n"
           << "  - ❌ No assumptions about missing or unclear instructions.\n"
           << "  - ❌ No content beyond what has been asked directly.\n"
           << "Respond ONLY with what is asked, structured exactly in the "
              "format and rules described.\n\n";
  } else {
    prompt << "Strict flag is DISABLED. Slight interpretation is allowed where "
              "unspecified, but core structure must still be preserved.\n\n";
  }
}

std::string generate_prompt(std::stringstream &out, Program *prgrm) {
  add_header(out);
  do_on(out, prgrm);
  add_sources(out, prgrm);
  add_conditions(out, prgrm);
  add_formatting(out, prgrm);
  add_strict(out, prgrm);
  add_footer(out);
  return out.str();
}
int main() {

  std::string code, line;
  std::cout << "Vakya (enter multiple lines, Ctrl+D to end input):\n";
  while (std::getline(std::cin, line)) {
    code += line + "\n"; // Preserve line breaks
  }

  std::stringstream prompt;
  Lexer lexer(code);
  AST ast(lexer);
  try {
    std::cout << lexer.make_tokens() << std::endl;
    ast.start_compiler();
    std::optional<Program *> prgrm = ast.get_program();
    if (prgrm.has_value()) {
      // ast.print_programs();
      std::cout << generate_prompt(prompt, *prgrm);
    }
  } catch (vakya_error &ve) {
    std::cout << ve.what() << "\n";
  }
}
