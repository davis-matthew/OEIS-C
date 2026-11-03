/*
 * A322810  Lexicographically earliest such sequence a that a(i) = a(j) =>
 *          f(i) = f(j) for all i, j, where f(n) = 0 if n is an odd squarefree
 *          number > 1, and f(n) = n for all other numbers.
 * https://oeis.org/A322810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, for n>2 if n is an odd square‑free integer >1,
 *            a(n)=3, otherwise a(n)=3 plus the number of indices i≤n such
 *            that i is not an odd square‑free integer
 * Range:     u8 n <= 425, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8181 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_odd_squarefree(idx_t n)
{
    if (n <= 1)           return false;        /* 1 is not considered odd square‑free >1 */
    if ((n & 1) == 0)     return false;        /* even numbers are not odd square‑free */

    /* Test for a squared prime factor */
    idx_t m = n;
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {                    /* p divides m */
            m /= p;
            if (m % p == 0)                 /* p² divides original n */
                return false;
            /* consume higher powers of p to reduce m */
            while (m % p == 0) m /= p;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (is_odd_squarefree(n)) return 3;

    seq_t count = 0;                          /* count of non‑odd‑square‑free indices up to n */
    for (idx_t i = 3; i <= n; ++i)
        if (!is_odd_squarefree(i))
            ++count;

    return 3 + count;
}
