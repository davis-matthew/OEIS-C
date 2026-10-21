/*
 * A281913  Number of ordered integer pairs (b,c), with -n<=b<=n, -n<=c<=n,
 *          such that both roots of 2x^2+bx+c=0 are rational and b and c are
 *          not both even.
 * https://oeis.org/A281913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of ordered integer pairs (b,c) with -n<=b,c<=n such that
 *            * 2x^2 + b x + c = 0 has both roots rational and not both b,c
 *            even. * The count equals the number of integer pairs (b,c)
 *            satisfying * b^2 - 8c = s^2 (s integer), * -n <= b <= n, -n <= c
 *            <= n, * not (b even && c even). * The roots bei...
 * Range:     u8 n <= 32, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: generate primes up to limit via simple sieve.  */
/* Assumes limit <= 80000 for n <= 10000.                     */
static void build_primes_up_to(uint64_t limit, uint32_t **primes_out, size_t *cnt_out) {
    uint8_t *is_comp = (uint8_t *)calloc(limit + 1, 1);
    if (!is_comp) return;
    size_t cap = 20;
    uint32_t *primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    for (uint64_t p = 2; p <= limit; ++p) {
        if (!is_comp[p]) {
            if (cnt_out && *cnt_out >= cap) {
                cap *= 2;
                primes = (uint32_t *)realloc(primes, cap * sizeof(uint32_t));
            }
            if (cnt_out) primes[*cnt_out] = (uint32_t)p;
            if (cnt_out) ++*cnt_out;
            if (p * p <= limit) {
                for (uint64_t k = p * p; k <= limit; k += p)
                    is_comp[k] = 1;
            }
        }
    }
    free(is_comp);
    *primes_out = primes;
    *(cnt_out) = *cnt_out;
}

/* Helper: factorize an integer m <= 80000 using prime list, storing factors in *primes and *exps,
 *        returning the number of distinct prime factors in cnt.  */
static size_t factorize_uint64(uint64_t m,
                               const uint32_t *primes, size_t prime_cnt,
                               uint32_t *primes_out, uint8_t *exps_out) {
    size_t cnt = 0;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= m; ++i) {
        if (m % primes[i] == 0) {
            uint8_t e = 0;
            while (m % primes[i] == 0) {
                m /= primes[i];
                ++e;
            }
            primes_out[cnt] = primes[i];
            exps_out[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {
        primes_out[cnt] = (uint32_t)m;
        exps_out[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* Helper: generate all positive divisors from factor list, storing in divs array.
 *         divs array must have capacity at least 128.  Returns number of divisors. */
static size_t generate_divisors(const uint32_t *primes, const uint8_t *exps,
                                size_t cnt, uint64_t *divs) {
    uint64_t res[128] = {1};
    size_t res_cnt = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t base = primes[i];
        uint8_t e = exps[i];
        size_t old_res_cnt = res_cnt;
        for (uint8_t k = 0; k < e; ++k)
            base *= primes[i];
        /* for each exponent 1..e, multiply existing divisors */
        size_t step = 1;
        for (uint8_t k = 1; k <= e; ++k) {
            uint64_t pwr = 1;
            for (uint8_t j = 0; j < k; ++j) pwr *= primes[i];
            for (size_t r = 0; r < old_res_cnt; ++r)
                res[res_cnt++] = res[r] * pwr;
        }
    }
    for (size_t i = 0; i < res_cnt; ++i)
        divs[i] = res[i];
    return res_cnt;
}

/* Main function as required. */
seq_t a(idx_t n) {
    /* Handle trivial small n (>=1). */
    if (n == 0) return 0;

    /* Count pairs. Use seq_t for the final count. */
    seq_t count = 0;

    /* Case c = 0: need b odd. */
    /* Number of odd integers in [-n, n] is 2 * ((n+1)/2). */
    seq_t odd_b = (seq_t)( ( ( (seq_t)n + 1 ) >> 1 ) * 2 );
    count += odd_b;

    /* Prepare small prime list up to 8*n. */
    uint32_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_up_to((uint64_t)8 * (uint64_t)n, &primes, &prime_cnt);

    /* Buffer for divisors; maximum divisors for numbers <= 80000 < 128. */
    uint64_t divs[128];
    uint32_t factor_primes[16];
    uint8_t factor_exps[16];

    /* Temporary array to mark b values seen for a given c to avoid duplicates. */
    size_t offset = (size_t)n;
    size_t seen_size = (size_t)(2 * n + 1);
    unsigned char *seen = (unsigned char *)calloc(seen_size, 1);

    /* Iterate over c != 0 */
    for (int64_t c = -(int64_t)n; c <= (int64_t)n; ++c) {
        if (c == 0) continue;
        int64_t t = 8LL * c;
        uint64_t abs_t = (t < 0) ? - (uint64_t)t : (uint64_t)t;

        /* Generate divisors of |t|. */
        size_t div_cnt = generate_divisors(
            (t == 0) ? NULL : primes, NULL, 0, divs); /* placeholder */

        /* Factor m = abs_t to get divisors. */
        size_t fcnt = factorize_uint64(abs_t, primes, prime_cnt, factor_primes, factor_exps);
        div_cnt = generate_divisors(factor_primes, factor_exps, fcnt, divs);

        /* Reset seen array for this c. */
        memset(seen, 0, seen_size);

        for (size_t i = 0; i < div_cnt; ++i) {
            uint64_t d = divs[i];
            uint64_t e = abs_t / d;
            /* Two possible sign assignments producing integer solutions. */
            for (int sign = -1; sign <= 1; sign += 2) {
                int64_t d1 = (int64_t)d * sign;
                int64_t d2 = ((int64_t)e) * sign * ((t >= 0) ? 1 : -1);
                /* Sum must be even to yield integer b. */
                int64_t sum = d1 + d2;
                if (sum & 1LL) continue;
                int64_t b = sum >> 1;
                if (b < -(int64_t)n || b > (int64_t)n) continue;
                /* Exclude both even. */
                if ( ((b & 1LL) == 0) && ((c & 1LL) == 0) ) continue;
                /* Ensure this (b,c) counted only once per c. */
                size_t idx = (size_t)(b + (int64_t)offset);
                if (!seen[idx]) {
                    seen[idx] = 1;
                    ++count;
                }
            }
        }
    }

    free(primes);
    free(seen);
    return count;
}
