/*
 * A290090  a(n) is the number of proper divisors of n that are odious
 *          (A000069).
 * https://oeis.org/A290090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of proper divisors of n that are odious * (odious
 *            = binary representation has an odd number of 1's)
 * Range:     u8 n <= 9798, u16 n <= 9798, u32 n <= 9798, u64 n <= 9798,
 *            u128 n <= 9798, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static bool primes_inited = false;
static uint32_t primes[1229];
static size_t primes_cnt = 0;

/* Simple sieve to generate all primes up to 10000. */
static void init_primes(void) {
    if (primes_inited) return;
    static bool is_composite[10001];
    for (size_t i = 2; i <= 10000; ++i) {
        if (!is_composite[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if (i * i <= 10000) {
                for (size_t j = i * i; j <= 10000; j += i)
                    is_composite[j] = true;
            }
        }
    }
    primes_inited = true;
}

/* Counts odious proper divisors of n. */
seq_t a(idx_t n) {
    if (n <= 1) return 0;           /* 1 has no proper divisors */

    init_primes();

    /* Factor n using the precomputed primes. */
    uint64_t temp = (uint64_t)n;
    uint32_t fac[16];
    int exp[16];
    size_t fcnt = 0;

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        if (temp % primes[i] == 0) {
            int e = 0;
            while (temp % primes[i] == 0) {
                temp /= primes[i];
                ++e;
            }
            fac[fcnt] = primes[i];
            exp[fcnt] = e;
            ++fcnt;
        }
    }
    if (temp > 1) {
        fac[fcnt] = (uint32_t)temp;
        exp[fcnt] = 1;
        ++fcnt;
    }

    /* Generate all divisors of n. */
    static uint64_t divs[256];
    size_t divs_len = 1;
    divs[0] = 1;

    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t p = fac[i];
        int e = exp[i];
        size_t old_len = divs_len;

        uint64_t mul = 1;
        for (int k = 1; k <= e; ++k) {
            mul *= p;
            for (size_t j = 0; j < old_len; ++j) {
                divs[divs_len++] = divs[j] * mul;
            }
        }
    }

    /* Count odious proper divisors. */
    seq_t count = 0;
    for (size_t i = 0; i < divs_len; ++i) {
        uint64_t d = divs[i];
        if (d == (uint64_t)n) continue;       /* exclude n itself */
        if (__builtin_popcountll(d) & 1)
            ++count;
    }
    return count;
}
