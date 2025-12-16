#pragma once
#include <vector>
#include <string>

/**
 * @struct LogoVariants
 * @brief Holds variants of a logo, sorted from Largest to Smallest.
 */
struct LogoVariants {
    std::vector<std::vector<std::string>> variants;
};

/**
 * @brief "Barty" Logo Variants
 */
extern const LogoVariants BARTY_VARIANTS;

/**
 * @brief "Pop!_OS" Logo Variants
 */
extern const LogoVariants POP_VARIANTS;

/**
 * @brief Helper to select the appropriate art variant based on constraints.
 * 
 * @param variants The logo variants to choose from.
 * @param force_small If true, always returns the small variant (e.g. for bouncing).
 * @param scr_width Screen Width constraint.
 * @param scr_height Screen Height constraint.
 * @return const std::vector<std::string>& The selected art.
 */
const std::vector<std::string>& select_art(const LogoVariants& variants, bool force_small, int scr_height, int scr_width);
