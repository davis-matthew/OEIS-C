/*
 * A255343  Numbers n such that there are exactly three 1's in their factorial
 *          base representation (A007623).
 * https://oeis.org/A255343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 6769, u64 n <= 6769, u128 n <= 6769,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of A255343: numbers whose factorial base
   representation contains exactly three 1's. The function enumerates
   integers in ascending order, checking each one's factorial base
   representation for exactly three digits equal to 1. */

#include <stdbool.h>

#define FACT_MAX 20   /* largest factorial needed for 64-bit values */

/* Lazily initialize factorial table 1! … FACT_MAX! */
static const seq_t *fact_table(void)
{
    static seq_t fact[FACT_MAX + 1];
    static bool init = false;
    if (!init) {
        fact[0] = 1;
        for (idx_t i = 1; i <= FACT_MAX; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        init = true;
    }
    return fact;
}

/* Return true iff val has exactly three digits equal to 1 in its
   factorial base representation. */
static bool has_three_ones(idx_t val, const seq_t *fact)
{
    seq_t x = (seq_t)val;
    int count = 0;
    idx_t k = FACT_MAX;
    while (k > 0) {
        seq_t d = x / fact[k];
        if (d == 1) {
            ++count;
            if (count > 3)
                return false;
        }
        x %= fact[k];
        --k;
    }
    return count == 3;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n = 0 – behaviour below offset not required */

    const seq_t *fact = fact_table();
    idx_t found = 0;
    for (idx_t val = 1; ; ++val) {
        if (has_three_ones(val, fact)) {
            ++found;
            if (found == n)
                return (seq_t)val;
        }
    }
}
