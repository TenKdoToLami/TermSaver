/**
 * @file AsciiArt.cpp
 * @brief Implementation of ASCII art data and selection logic.
 */

#include "AsciiArt.hpp"

// BARTY LOGOS -----------------------------------------------------------------
// Sorted Largest -> Smallest
const LogoVariants BARTY_VARIANTS = {{
    // Large
    {
        "     ##### ##                                                  ", 
        "  ######  /##                                                  ", 
        " /#   /  / ##                              #                   ", 
        " /    /  /  ##                             ##                  ", 
        "    /  /   /                              ##                   ", 
        "   ## ##  /        /###   ###  /###     ######## ##   ####     ", 
        "   ## ## /        / ###  / ###/ #### / ########   ##    ###  / ", 
        "   ## ##/        /   ###/   ##   ###/     ##      ##     ###/  ", 
        "   ## ## ###    ##    ##    ##            ##      ##      ##   ", 
        "   ## ##   ###  ##    ##    ##            ##      ##      ##   ", 
        "   #  ##     ## ##    ##    ##            ##      ##      ##   ", 
        "      /      ## ##    ##    ##            ##      ##      ##   ", 
        "  /##/     ###  ##    /#    ##            ##      ##      ##   ", 
        " /  ########     ####/ ##   ###           ##       #########   ", 
        "/     ####        ###   ##   ###           ##        #### ###  ", 
        "#                                                          ### ", 
        " ##                                                 #####   ###", 
        "                                                  /#######  /# ", 
        "                                                 /      ###/   "
    },
    // Medium
    {
        "888888b.                    888            ",
        "888  \"88b                   888            ",
        "888  .88P                   888            ",
        "8888888K.   8888b.  888d888 888888 888  888",
        "888  \"Y88b     \"88b 888P\"   888    888  888",
        "888    888 .d888888 888     888    888  888",
        "888   d88P 888  888 888     Y88b.  Y88b 888",
        "8888888P\"  \"Y888888 888      \"Y888  \"Y88888",
        "                                        888",
        "                                   Y8b d88P",
        "                                    \"Y88P\" "
    },
    // Small
    {
        "  ____    _    ____  _____ __   __  ",
        " | __ )  / \\  |  _ \\|_   _|\\ \\ / /  ",
        " |  _ \\ / _ \\ | |_) | | |   \\ V /   ",
        " | |_) / ___ \\|  _ <  | |    | |    ",
        " |____/_/   \\_\\_| \\_\\ |_|    |_|    "
    }
}};

// POP LOGOS -------------------------------------------------------------------
const LogoVariants POP_VARIANTS = {{
    // Small
    {
        "             /////////////             ",
        "         /////////////////////         ",
        "      ///////*7676////////////////     ",
        "    //////7676767676*//////////////    ",
        "   /////76767//7676767//////////////   ",
        "  /////767676///76767///////////////   ",
        " ///////767676///76767.///7676*/////// ",
        "/////////767676//76767///767676////////",
        "//////////76767676767////76767/////////",
        "///////////76767676//////7676//////////",
        "////////////,7676,///////767///////////",
        "/////////////*7676///////76////////////",
        "///////////////7676////////////////////",
        "///////////////7676///767////////////  ",
        "  //////////////////////'////////////  ",
        "   //////.7676767676767676767,//////   ",
        "    /////767676767676767676767/////    ",
        "      ///////////////////////////      ",
        "         /////////////////////         ",
        "             /////////////             "
    }
}};


// LOGIC -----------------------------------------------------------------------

namespace {
    void get_dims(const std::vector<std::string>& art, int& w, int& h) {
        h = art.size();
        w = 0;
        for (const auto& s : art) {
            if ((int)s.length() > w) w = s.length();
        }
    }
}

const std::vector<std::string>& select_art(const LogoVariants& variants, bool force_small, int scr_height, int scr_width) {
    if (variants.variants.empty()) {
        static const std::vector<std::string> EMPTY;
        return EMPTY;
    }

    if (force_small) {
        // Return the LAST element (Smallest)
        return variants.variants.back();
    }
    
    // Iterate from Largest (0) to Smallest (end)
    // Return first one that fits
    for (const auto& art : variants.variants) {
        int w, h;
        get_dims(art, w, h);
        if (w < scr_width && h < scr_height) {
            return art;
        }
    }
    
    // If none fit, return the smallest (last one) as a fallback
    return variants.variants.back();
}
