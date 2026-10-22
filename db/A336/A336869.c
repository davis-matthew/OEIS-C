/*
 * A336869  Number of divisors d of n! with distinct prime multiplicities such
 *          that the quotient n!/d also has distinct prime multiplicities.
 * https://oeis.org/A336869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336869: number of divisors d of n! with pairwise distinct
 * prime exponents such that n!/d also has pairwise distinct prime exponents.
 * For n >= 17 the value is 0.  The implementation enumerates all possible
 * exponent splits for the primes <= n, which is feasible because n <= 16
 * in this case.  The function works for any seq_t and idx_t defined by
 * the build environment. */
#include <stdbool.h>
#include <stdint.h>

static void dfs_count(int pos, int prime_cnt, const uint8_t *e,
                      bool *usedD, bool *usedC, seq_t *cnt)
{
    if (pos == prime_cnt) {
        (*cnt)++;
        return;
    }
    uint8_t ei = e[pos];
    for (uint8_t fi = 0; fi <= ei; ++fi) {
        uint8_t dexp = fi;
        uint8_t cexp = ei - fi;
        if ((dexp > 0 && usedD[dexp]) ||
            (cexp > 0 && usedC[cexp]))
            continue;
        if (dexp > 0) usedD[dexp] = true;
        if (cexp > 0) usedC[cexp] = true;
        dfs_count(pos + 1, prime_cnt, e, usedD, usedC, cnt);
        if (dexp > 0) usedD[dexp] = false;
        if (cexp > 0) usedC[cexp] = false;
    }
}

seq_t a(idx_t n)
{
    /* For n >= 17 the sequence is identically 0. */
    if (n > 16) return (seq_t)0;

    /* Small list of primes up to 16. */
    static const uint32_t prime_list[6] = {2, 3, 5, 7, 11, 13};
    static const int max_prime_count = 6;

    /* e[i] holds the exponent of prime_list[i] in n! */
    uint8_t e[max_prime_count];
    int prime_cnt = 0;

    for (int i = 0; i < max_prime_count; ++i) {
        uint32_t p = prime_list[i];
        if (p > (uint32_t)n) break;
        uint8_t exp = 0;
        idx_t div = n / p;
        while (div) {
            exp += (uint8_t)div;
            div /= p;
        }
        e[prime_cnt++] = exp;
    }

    /* Used exponent flags for d and for n!/d. Max exponent for n<=16 is 15. */
    bool usedD[32] = {0};
    bool usedC[32] = {0};
    seq_t result = (seq_t)0;

    dfs_count(0, prime_cnt, e, usedD, usedC, &result);
    return result;
}
