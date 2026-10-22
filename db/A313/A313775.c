/*
 * A313775  Coordination sequence Gal.5.329.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A313775: Coordination sequence Gal.5.329.2
   Formula: a(n) = a(n-6) + 32   for n >= 16
   Initial values (n = 0..15):
   1, 5, 10, 15, 21, 25, 31, 37, 42, 47, 54, 58, 64, 70, 74, 80
*/

#include <stddef.h>

static const seq_t init_vals[24] = {
    1ULL,   /* 0 */
    5ULL,   /* 1 */
    10ULL,  /* 2 */
    15ULL,  /* 3 */
    21ULL,  /* 4 */
    25ULL,  /* 5 */
    31ULL,  /* 6 */
    37ULL,  /* 7 */
    42ULL,  /* 8 */
    47ULL,  /* 9 */
    54ULL,  /* 10 */
    58ULL,  /* 11 */
    64ULL,  /* 12 */
    70ULL,  /* 13 */
    74ULL,  /* 14 */
    80ULL,  /* 15 */
    86ULL,  /* 16 */
    90ULL,  /* 17 */
    96ULL,  /* 18 */
    102ULL, /* 19 */
    106ULL, /* 20 */
    112ULL, /* 21 */
    118ULL, /* 22 */
    122ULL  /* 23 */
};

seq_t a(idx_t n) {
    /* For very small n use the pre‑computed table. */
    if (n < 24) {
        return init_vals[n];
    }

    /* n >= 16 – use the linear recurrence a(n)=a(n-6)+32. */
    static const idx_t base_map[6] = {
        18, /* n % 6 == 0 */
        19, /* n % 6 == 1 */
        20, /* n % 6 == 2 */
        21, /* n % 6 == 3 */
        16, /* n % 6 == 4 */
        17  /* n % 6 == 5 */
    };

    idx_t mod = n % 6;
    idx_t base = base_map[mod];

    /* The recurrence only holds for n >= 16; the table covers up to 23,
       so base will always be <= n for n >= 16. */
    if (base > n) {
        return init_vals[n];
    }

    seq_t base_val = init_vals[base];
    idx_t steps = (n - base) / 6;
    seq_t add = (seq_t)steps * (seq_t)32;

    return base_val + add;
}
