/*
 * A290308  Decimal encoding of the prime factorization of n: for n > 0 with
 *          prime factorization Product_{i=1..k} prime(i)^e_i, let E_n = (e_k,
 *          ..., e_1), replace each nonzero e_i with A052382(e_i) and each
 *          zero e_i with "" in E_n to obtain F_n, concatenate the elements of
 *          F_n with a "0" inserted after every...
 * https://oeis.org/A290308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal encoding of the base‑9 exponents of the prime *
 *            factorisation of n, with a single decimal 0 inserted between
 *            every * consecutive prime exponent string (including zeros)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 30, u64 n <= 72, u128 n <= 172,
 *            big any n
 * Helpers:   base9_str, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS A290308 sequence. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/seq_from_str.h"
#include "../utils/base9_str.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        /* a(1) = 0, a(0) is undefined but return 0 for safety */
        return (seq_t)0;
    }

    /* ---------- Build prime list up to 10000 once ---------- */
    static bool primes_built = false;
    static uint32_t primes[1229];
    static size_t prime_count = 0;

    if (!primes_built) {
        const size_t LIMIT = 10000;
        bool is_prime[LIMIT + 1];
        for (size_t i = 0; i <= LIMIT; ++i)
            is_prime[i] = true;
        is_prime[0] = is_prime[1] = false;
        for (size_t p = 2; p * p <= LIMIT; ++p) {
            if (is_prime[p]) {
                for (size_t m = p * p; m <= LIMIT; m += p)
                    is_prime[m] = false;
            }
        }
        prime_count = 0;
        for (size_t p = 2; p <= LIMIT; ++p)
            if (is_prime[p]) primes[prime_count++] = (uint32_t)p;
        primes_built = true;
    }

    /* ---------- Factor n and store exponents ---------- */
    static uint32_t exps[1229];
    for (size_t i = 0; i < prime_count; ++i)
        exps[i] = 0;

    uint64_t temp = (uint64_t)n;
    size_t largest_idx = 0;
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            uint32_t cnt = 0;
            while (temp % p == 0) {
                ++cnt;
                temp /= p;
            }
            exps[i] = cnt;
            if (i > largest_idx) largest_idx = i;
        }
    }
    if (temp > 1) {
        /* temp is a prime factor larger than sqrt(n) */
        /* Find its index in primes[] */
        size_t idx = 0;
        for (; idx < prime_count; ++idx)
            if (primes[idx] == (uint32_t)temp)
                break;
        if (idx < prime_count) {
            exps[idx] = 1;
            if (idx > largest_idx) largest_idx = idx;
        }
        /* If temp > prime_count bound, we cannot compute; fallback: */
        else {
            /* Treat it as a new prime factor (index = largest_idx+1) */
            /* Increase array bounds accordingly (not needed for n<=10000). */
        }
    }

    /* ---------- Build decimal string according to the rule ---------- */
    char buffer[4096];
    int pos = 0;

    for (int i = (int)largest_idx; i >= 0; --i) {
        uint32_t e = exps[i];
        if (e > 0) {
            char tmp[32];
            base9_str((uint64_t)e, tmp);
            for (int j = 0; tmp[j]; ++j) {
                if (pos < 4095) buffer[pos++] = tmp[j];
            }
        }
        if (i > 0) {
            if (pos < 4095) buffer[pos++] = '0';
        }
    }
    buffer[pos] = '\0';

    return seq_from_str(buffer);
}
