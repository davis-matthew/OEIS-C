/*
 * A321779  Number of compositions of n into parts with distinct
 *          multiplicities and with exactly nine parts.
 * https://oeis.org/A321779
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into 9 parts with distinct
 *            multiplicities
 * Range:     u8 n <= 12, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Patterns of distinct multiplicities that sum to 9.  Each pattern specifies
   the multiset of multiplicities and the multinomial coefficient
   9!/(m1! m2! ... mk!). */
typedef struct {
    idx_t w[3];      /* multiplicities; unused slots are 0 */
    idx_t k;         /* how many distinct part values the pattern has */
    seq_t coeff;     /* multinomial coefficient for this pattern */
} pattern_t;

static const pattern_t patterns[] = {
    { {9,0,0}, 1, 1            },
    { {8,1,0}, 2, 9            },
    { {7,2,0}, 2, 36           },
    { {6,3,0}, 2, 84           },
    { {6,2,1}, 3, 252          },
    { {5,4,0}, 2, 126          },
    { {5,3,1}, 3, 504          },
    { {4,3,2}, 3, 1260         }
};

/* Count of compositions of n into 9 parts with distinct multiplicities */
seq_t a(idx_t n)
{
    if (n < 9) return 0;

    seq_t res = 0;

    /* [9] pattern – only one part value repeated nine times. */
    if (n % 9 == 0) {            /* the part value is n/9, unique */
        res += 1;                /* coefficient is 1 */
    }

    /* Remaining patterns: [8,1], [7,2], [6,3], [6,2,1], [5,4], [5,3,1], [4,3,2] */
    for (size_t pi = 1; pi < sizeof(patterns)/sizeof(patterns[0]); ++pi) {
        const pattern_t *pat = &patterns[pi];
        if (pat->k == 2) {          /* patterns of the form [a,b] */
            idx_t a_mult = pat->w[0];
            idx_t b_mult = pat->w[1];

            seq_t cnt = 0;
            for (idx_t x = 1; a_mult * x < n; ++x) {
                idx_t rem = n - a_mult * x;
                if (rem % b_mult != 0) continue;   /* y not integer */
                idx_t y = rem / b_mult;
                if (y == 0 || y == x) continue;    /* distinct and positive */
                ++cnt;
            }
            res += cnt * pat->coeff;
        } else if (pat->k == 3) {    /* patterns of the form [a,b,c] */
            idx_t a_mult = pat->w[0];
            idx_t b_mult = pat->w[1];
            idx_t c_mult = pat->w[2];

            seq_t cnt = 0;
            for (idx_t x = 1; a_mult * x < n; ++x) {
                idx_t rem1 = n - a_mult * x;
                for (idx_t y = 1; b_mult * y < rem1; ++y) {
                    idx_t rem2 = rem1 - b_mult * y;
                    if (rem2 % c_mult != 0) continue;
                    idx_t z = rem2 / c_mult;
                    if (z == 0) continue;
                    if (x == y || x == z || y == z) continue; /* all distinct */
                    ++cnt;
                }
            }
            res += cnt * pat->coeff;
        }
    }
    return res;
}
