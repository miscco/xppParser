#ifndef XPPCOLOR_H
#define XPPCOLOR_H

/**
 * @brief The xppColor enum defines the possible line colors utilized by xppaut
 */
enum xppColor {
    WHITE,
    RED,
    REDORANGE,
    ORANGE,
    YELLOWORANGE,
    YELLOW,
    YELLOWGREEN,
    GREEN,
    BLUEGREEN,
    BLUE,
    PURPLE,
    BLACK
};

/**
 * @brief The xppColorMap enum defines the different colormaps that can be used
 */
enum xppColorMap {
    C_NORM,
    C_PERIODIC,
    C_HOT,
    C_COOL,
    C_REDBLUE,
    C_GRAY,
    C_CUBHLX
};

/**
 * @brief The xppMainColor struct holds the chosen color settings for the main
 * window.
 */
struct xppMainColor {
    /**
     * @brief The colors manifolds and nullclines are displayed in
     */
    xppColor stableManifoldColor	= BLACK;
    xppColor unstableManifoldColor	= BLACK;
    xppColor xNullclineColor		= BLACK;
    xppColor yNullclineColor		= BLACK;

    /**
     * @brief colorMap the utilized color map
     */
    xppColorMap colorMap = C_NORM;
};


/**
 * @brief The xppAutoColor struct holds the chosen color settings for the auto
 * window.
 */
struct xppAutoColor {
    /**
     * @brief The colors equilibria and orbits are displayed in
     */
    xppColor stableEquilibriumColor	  = RED;
    xppColor unstableEquilibriumColor = BLACK;
    xppColor stablePeriodicColor	  = GREEN;
    xppColor unstablePeriodicColor	  = BLUE;
};

#endif // XPPCOLOR_H

