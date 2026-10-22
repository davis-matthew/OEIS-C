/*
 * A326028  Number of factorizations of n into factors > 1 with integer
 *          geometric mean.
 * https://oeis.org/A326028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9597, u16 n <= 9597, u32 n <= 9597, u64 n <= 9597,
 *            u128 n <= 9597, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) counts unordered factorizations of n into factors >1
 * whose geometric mean is integer.  For a given n, let
 * n = ∏ p_i^e_i and let G = gcd(e_1, e_2, …).  Then n is a perfect d‑th
 * power iff d divides G.  For each such d we count unordered
 * factorizations of n into exactly d factors >1.  a(n) is the sum of
 * these counts.  The computation is performed by recursively generating
 * the factorizations in non‑decreasing order of factors.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* Helper: greatest common divisor for idx_t */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Recursive enumeration of factorizations of 'cur' into >1 factors
   with each factor not less than 'min_factor'.
   'len' is the number of factors chosen so far.
   cnts[len] is incremented whenever a complete factorization ends. */
static void dfs(idx_t cur, idx_t min_factor, idx_t len, seq_t *cnts)
{
    /* Generate all divisors of cur greater than 1 (unordered). */
    idx_t divs[128];
    size_t div_cnt = 0;

    /* Divisors found by iterating up to sqrt(cur). */
    for (idx_t d = 2; d * d <= cur; ++d) {
        if (cur % d == 0) {
            divs[div_cnt++] = d;
            if (d != cur / d)
                divs[div_cnt++] = cur / d;
        }
    }
    /* Include cur itself. */
    divs[div_cnt++] = cur;

    /* Enumerate divisors respecting non-decreasing order. */
    for (size_t i = 0; i < div_cnt; ++i) {
        idx_t d = divs[i];
        if (d < min_factor)
            continue;
        idx_t next = cur / d;
        if (next == 1) {
            cnts[len] += 1;
        } else {
            dfs(next, d, len + 1, cnts);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    /* Factor n to obtain the exponents of its prime factorization. */
    idx_t temp = n;
    idx_t G = 0;          /* gcd of exponents */
    int first = 1;

    /* Small prime list up to 100 to factor n (n <= 10000). */
    static const idx_t primes[25] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97
    };

    for (size_t i = 0; i < 25 && primes[i] * primes[i] <= temp; ++i) {
        idx_t p = primes[i];
        if (temp % p == 0) {
            idx_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            G = first ? cnt : gcd_idx(G, cnt);
            first = 0;
        }
    }
    if (temp > 1) {             /* remaining prime factor */
        G = first ? 1 : gcd_idx(G, 1);
    }

    /* If G==0, n was 1 which is handled above. */
    /* Count all unordered factorizations of n into >1 factors. */
    seq_t cnts[128];
    memset(cnts, 0, sizeof(cnts));
    dfs(n, 2, 0, cnts);

    /* Sum counts for lengths d that divide G. */
    seq_t result = 0;
    for (idx_t d = 1; d <= G; ++d) {
        if (G % d == 0 && (d - 1) < sizeof(cnts)/sizeof(cnts[0]))
            result += cnts[d - 1];
    }
    return result;
}
