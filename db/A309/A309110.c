/*
 * A309110  Number of possible permutations of a Pyraminx of size n, including
 *          the trivial rotation of the tips.
 * https://oeis.org/A309110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) * 272097792 * 369600^(2*n-6) for n >= 6. Initial
 *            values (offset 1): a(1) = 1 a(2) = 81 a(3) = 75582720 a(4) =
 *            217225462874112000 a(5) = 19228688422470957567836160000000
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, seq_from_str, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_from_str.h"

static bool initialized = false;
static seq_t base_vals[6];  /* 0 unused, indices 1..5 */

/* Initialize the static base values once */
static void init_base_vals(void)
{
    if (initialized)
        return;
    base_vals[1] = (seq_t)1ULL;
    base_vals[2] = (seq_t)81ULL;
    base_vals[3] = (seq_t)75582720ULL;
    base_vals[4] = (seq_t)217225462874112000ULL;
    /* 19228688422470957567836160000000 too large for 64‑bit */
    base_vals[5] = seq_from_str(
        "19228688422470957567836160000000");
    initialized = true;
}

/*
   Return a(n) for the Pyraminx permutation count
   (OEIS A309110).  n is 1‑based index.
*/
seq_t a(idx_t n)
{
    init_base_vals();

    if (n == 0)    /* out of range, but return 0 for safety */
        return (seq_t)0;

    /* small n handled directly */
    if (n <= 5)
        return base_vals[n];

    /* general case: use the 3‑step recurrence */
    idx_t r = (n - 3) % 3;          /* 0,1,2 */
    idx_t base_index = 3 + r;       /* 3,4 or 5 */
    idx_t m = (n - base_index) / 3; /* number of recurrence steps */

    seq_t result = base_vals[base_index];

    if (m == 0)  /* should not happen for n>5, but keep safe */
        return result;

    /* exponent for 369600:  sum_{i=1..m} (2*(base+3i)-6) */
    seq_t two_base_minus6 = (seq_t)((2 * base_index) - 6);
    seq_t t1 = (seq_t)m * two_base_minus6;
    seq_t t2 = (seq_t)3 * (seq_t)m * (seq_t)(m + 1);
    seq_t exponent_for_base = t1 + t2;   /* exponent for 369600 */

    /* compute the multiplicative factors */
    seq_t factor_power = big_pow((seq_t)272097792, (seq_t)m);
    seq_t base_factor  = big_pow((seq_t)369600, exponent_for_base);

    /* result = result * factor_power * base_factor */
    seq_t temp = seq_mul(result, factor_power);
    result = seq_mul(temp, base_factor);

    return result;
}
