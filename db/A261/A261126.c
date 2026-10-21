/*
 * A261126  Number of mora in Japanese name of n in 'On' reading. In case
 *          there are several spellings, use the shorter one.
 * https://oeis.org/A261126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, more
 * Formula:   a(n) is the number of mora in the Japanese “On” reading of n
 *            (using the shortest available spelling for each digit and fixed
 *            pronunciations for ten, hundred and thousand)
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case for 0: “rei” */
    if (n == 0)
        return (seq_t)2;          /* “rei” has 2 mora */

    /* Short digit readings (only used for the units place when n == 9,  
       otherwise long readings are used for 9). */
    static const unsigned char DIGIT_SHORT[10] = {
        2, /* 0 “rei”   */
        2, /* 1 “ichi”  */
        1, /* 2 “ni”    */
        2, /* 3 “san”   */
        1, /* 4 “shi”   */
        1, /* 5 “go”    */
        2, /* 6 “roku”  */
        2, /* 7 “nana”  */
        2, /* 8 “hachi” */
        1  /* 9 “ku” (short) */
    };

    seq_t sum     = 0;
    idx_t r       = n;      /* Remaining part to process */
    idx_t g_index = 0;      /* Current 4‑digit group (units = 0) */

    while (r > 0) {
        idx_t group = r % (idx_t)10000;
        r /= (idx_t)10000;

        seq_t g_mora = 0;

        /* ---- thousands (d3) ---- */
        idx_t d3 = group / 1000;
        if (d3 != 0) {
            if (d3 == 1)
                g_mora += (seq_t)2;                     /* sen */
            else
                g_mora += (seq_t)((d3 == 9) ? 2 : DIGIT_SHORT[d3]) + (seq_t)2;
        }

        /* ---- hundreds (d2) ---- */
        idx_t d2 = (group / 100) % 10;
        if (d2 != 0) {
            if (d2 == 1)
                g_mora += (seq_t)2;                     /* hyaku */
            else
                g_mora += (seq_t)((d2 == 9) ? 2 : DIGIT_SHORT[d2]) + (seq_t)2;
        }

        /* ---- tens (d1) ---- */
        idx_t d1 = (group / 10) % 10;
        if (d1 != 0) {
            if (d1 == 1) {                               /* “juu” alone */
                g_mora += (seq_t)1;                     /* juu (shortened form) */
            } else {
                g_mora += (seq_t)1;                     /* juu */
                g_mora += (seq_t)((d1 == 9) ? 2 : DIGIT_SHORT[d1]); /* digit word */
            }
        }

        /* ---- units (d0) ---- */
        idx_t d0 = group % 10;
        if (d0 != 0) {
            if (d0 == 9 && n == 9 && g_index == 0) {
                g_mora += (seq_t)1;                     /* short “ku” for 9 itself */
            } else {
                g_mora += (seq_t)((d0 == 9) ? 2 : DIGIT_SHORT[d0]); /* long “kyuu” or other digit */
            }
        }

        /* Skip digit reading for the most significant group that is exactly 1 */
        if (r == 0 && group == 1 && g_index > 0)
            g_mora = 0;

        sum += g_mora;

        /* Add the unit word (“man”, “oku”, …) for non‑zero groups except the units group */
        if (g_index > 0 && group != 0)
            sum += (seq_t)1;

        ++g_index;
    }

    return sum;
}
