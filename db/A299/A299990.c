/*
 * A299990  a(n) = A243822(n) - A000005(n).
 * https://oeis.org/A299990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A010846(n) – 2·A000005(n) where A010846(n) = number of
 *            integers ≤ n whose prime factors all belong to the prime
 *            factorization of n, and A000005(n) = divisor count of n
 * Range:     i8 n <= 2309, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* factor n into prime powers.
   primes   : array to receive prime bases (max 16 primes suffices for all n < 2⁶⁴).
   exps     : array to receive corresponding exponents.
   returns  : number of distinct primes found.                */
static size_t factor_uint64(uint64_t n, uint64_t primes[], int exps[]) {
    size_t cnt = 0;
    if (n < 2) return cnt;
    if (n % 2 == 0) {
        int e = 0;
        while (n % 2 == 0) { n /= 2; ++e; }
        primes[cnt] = 2;
        exps[cnt]   = e;
        ++cnt;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (n > 1) {
        primes[cnt] = n;
        exps[cnt]   = 1;
        ++cnt;
    }
    return cnt;
}

/* recursive enumeration of all n‑regular numbers ≤ limit.
   idx     : current prime index in primes[];
   cur     : current product (always ≤ limit).
   primes  : array of prime bases.
   primes_len : number of primes in primes[].
   limit   : upper bound (original n).
   cnt     : pointer to accumulator (seq_t).              */
static void dfs(size_t idx, uint64_t cur, size_t primes_len,
                const uint64_t primes[], idx_t limit, seq_t *cnt) {
    if (cur > (uint64_t)limit) return;
    if (idx == primes_len) {        /* leaf – cur is a valid n‑regular number */
        ++(*cnt);
        return;
    }
    uint64_t val = cur;
    while (1) {
        dfs(idx + 1, val, primes_len, primes, limit, cnt);
        if (val > (uint64_t)limit / primes[idx]) break;
        val *= primes[idx];
    }
}

/* Main entry for sequence A299990. */
seq_t a(idx_t n) {
    uint64_t primes[16];
    int exps[16];
    size_t pcnt = factor_uint64((uint64_t)n, primes, exps);

    /* divisor count d(n) = Π (exp_i + 1) */
    seq_t dcnt = 1;
    for (size_t i = 0; i < pcnt; ++i)
        dcnt *= (seq_t)(exps[i] + 1);

    /* count of n‑regular numbers ≤ n */
    seq_t reg_cnt = 0;
    dfs(0, 1ULL, pcnt, primes, n, &reg_cnt);

    /* a(n) = reg_cnt – 2·dcnt */
    return reg_cnt - 2 * dcnt;
}
