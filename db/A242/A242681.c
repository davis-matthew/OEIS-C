/*
 * A242681  The number of ways that a score of n can be obtained using two
 *          darts on a standard dartboard.
 * https://oeis.org/A242681
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = #{(x,y) | x,y are attainable single-dart scores, x <= y,
 *            x + y = n}
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

enum { MAX_SCORE = 60 };          /* highest possible score with one dart */

static bool reachable[MAX_SCORE + 1];
static uint8_t scores[MAX_SCORE + 1];
static size_t score_len = 0;
static bool initialized = false;

static void init_scores(void)
{
    for (int i = 0; i <= MAX_SCORE; ++i)
        reachable[i] = false;

    for (int k = 1; k <= 20; ++k) {
        reachable[k] = true;                    /* single */
        if (2 * k <= MAX_SCORE)
            reachable[2 * k] = true;            /* double */
        if (3 * k <= MAX_SCORE)
            reachable[3 * k] = true;            /* triple */
    }
    if (25 <= MAX_SCORE) reachable[25] = true;    /* outer bull */
    if (50 <= MAX_SCORE) reachable[50] = true;    /* inner bull */

    score_len = 0;
    for (int i = 1; i <= MAX_SCORE; ++i)
        if (reachable[i])
            scores[score_len++] = (uint8_t)i;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_scores();
        initialized = true;
    }

    if (n < 2 || n > 2uLL * MAX_SCORE)
        return 0;

    seq_t count = 0;
    idx_t half = n >> 1;              /* floor(n / 2) */

    for (size_t i = 0; i < score_len; ++i) {
        uint8_t x = scores[i];
        if ((idx_t)x > half)
            break;                     /* further x would make x > y */

        idx_t y = n - (idx_t)x;
        if (y < (idx_t)x)
            continue;                  /* already counted when y > x */

        if (y > MAX_SCORE)
            continue;                  /* y not attainable with one dart */

        if (reachable[(size_t)y])
            ++count;
    }

    return count;
}
