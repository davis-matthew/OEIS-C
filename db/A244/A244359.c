/*
 * A244359  Numbers n such that n, n+1, n+2, n+3, and n+4 are not divisible by
 *          any of their nonzero digits.
 * https://oeis.org/A244359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Identify numbers n such that each of n, n+1, n+2, n+3, n+4 has
 *            no divisor among its non‑zero digits. The function computes the
 *            nth term (offset 1) by incremental brute‑force search, caching
 *            previously found terms so that calls can be in any order
 * Range:     u8 none, u16 n <= 7, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>   /* size_t */

/* Digit‑divisor test for a number */
static bool valid_number(seq_t x)
{
    seq_t t = x;
    while (t != 0) {
        seq_t d = t % 10;          /* obtain a decimal digit */
        if (d != 0) {
            if (x % d == 0)        /* divisible by a non‑zero digit */
                return false;
        }
        t = t / 10;                /* drop the lowest digit */
    }
    return true;
}

/* Cache for sequence values (enough for n up to 20000) */
static seq_t      terms[20000];
static size_t     terms_len = 0;     /* number of elements stored */
static seq_t      next_candidate = 6;  /* next number ending in 6 to test */

/* Return the nth term of sequence A244359 (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined per OEIS offset; return 0 safely */

    /* Generate terms up to n if necessary */
    while (terms_len < n && terms_len < 20000) {
        seq_t cand = next_candidate;
        bool good = true;

        /* Check the 5 consecutive integers cand … cand+4 */
        for (int i = 0; i < 5 && good; ++i) {
            seq_t x = cand + i;
            if (!valid_number(x))
                good = false;
        }

        if (good)
            terms[terms_len++] = cand;

        next_candidate += 10;   /* move to the next number ending in 6 */
    }

    return terms[n - 1];
}
