/*
 * A265349  Numbers in whose factorial base representation (A007623) no digit
 *          > 0 occurs more than once.
 * https://oeis.org/A265349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 4551, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 90 terms through n = 9580 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generating A265349 by brute‑force search: numbers whose
   factorial‑base representation has no repeated non‑zero digits. */
#include <stdbool.h>

/* Test whether v satisfies the factorial‑base uniqueness property. */
static bool factorial_base_unique(seq_t v)
{
    if (v == 0) return true;          /* 0 is a special valid element   */
    bool seen[1024] = {0};            /* track which digits have appeared */
    seq_t x = v;
    int k = 1;
    while (x != 0) {
        seq_t digit = x % (k + 1);
        if (digit != 0) {
            unsigned long long d = (unsigned long long)digit;
            if (d >= 1024) return false;        /* digit out of range – impossible for required n */
            if (seen[d]) return false;          /* repeated non‑zero digit */
            seen[d] = true;
        }
        x /= (k + 1);
        k++;
    }
    return true;
}

/* Return the n‑th term of A265349 (0‑indexed). */
seq_t a(idx_t n)
{
    idx_t count = 0;   /* number of valid terms found */
    seq_t v = 0;       /* current integer to test */

    while (count <= n) {
        if (factorial_base_unique(v)) {
            if (count == n) return v;
            count++;
        }
        v++;            /* test next integer */
    }
    /* control should never reach here */
    return 0;
}
