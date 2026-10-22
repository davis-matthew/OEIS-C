/*
 * A326514  Number of factorizations of n into factors > 1 where each factor
 *          has a different number of prime factors counted with multiplicity.
 * https://oeis.org/A326514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of factorizations of n into factors > 1 where each
 *            factor has a distinct value of ω (the number of prime factors
 *            counted with multiplicity). The function enumerates all
 *            factorizations with non‑decreasing factors to avoid repetitions
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static idx_t spf_limit = 0;
static uint64_t *spf_arr  = NULL;   /* smallest prime factor */
static uint8_t  *omega_arr = NULL;  /* ω(n) for n <= spf_limit */

/* ensure that spf_arr, omega_arr are valid up to at least n */
static void ensure_limit(idx_t n) {
    if (n <= spf_limit) return;

    idx_t new_limit = spf_limit ? spf_limit * 2 : 2;
    while (new_limit < n) new_limit <<= 1;

    uint64_t *new_spf = (uint64_t*)malloc((new_limit + 1) * sizeof(uint64_t));
    uint8_t  *new_omega = (uint8_t*)malloc((new_limit + 1) * sizeof(uint8_t));
    if (!new_spf || !new_omega) abort();

    if (spf_limit > 0) {
        memcpy(new_spf, spf_arr, (spf_limit + 1) * sizeof(uint64_t));
        memcpy(new_omega, omega_arr, (spf_limit + 1) * sizeof(uint8_t));
    } else {
        memset(new_spf, 0, (new_limit + 1) * sizeof(uint64_t));
        memset(new_omega, 0, (new_limit + 1) * sizeof(uint8_t));
    }
    free(spf_arr);
    free(omega_arr);
    spf_arr   = new_spf;
    omega_arr = new_omega;

    /* sieve smallest prime factor */
    for (idx_t i = 2; i <= new_limit; ++i) spf_arr[i] = i;
    for (idx_t i = 2; i * (idx_t)i <= new_limit; ++i) {
        if (spf_arr[i] == (uint64_t)i) {
            for (idx_t j = i * i; j <= new_limit; j += i) {
                if (spf_arr[j] == (uint64_t)j) spf_arr[j] = i;
            }
        }
    }

    /* compute ω (prime factors with multiplicity) */
    omega_arr[0] = 0;
    omega_arr[1] = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        omega_arr[i] = omega_arr[i / spf_arr[i]] + 1;
    }

    spf_limit = new_limit;
}

/* recursive search for factorizations of rem, with previous factor at least last,
   and mask of already used ω values stored in used (bit i set -> ω=i used). */
static seq_t dfs(uint64_t rem, uint64_t last, uint64_t used) {
    if (rem == 1) return 1;  /* reached a complete factorization */

    seq_t total = 0;

    /* option: take the whole remaining product as the last factor */
    if (rem >= last) {
        uint8_t om = omega_arr[rem];
        uint64_t bit = (om < 64) ? (1ull << om) : 0;
        if (om < 64 && !(used & bit))
            total += (seq_t)1;
    }

    /* enumerate proper divisors d <= sqrt(rem) that respect the non‑decreasing order */
    for (uint64_t d = last; d * d <= rem; ++d) {
        if (rem % d != 0) continue;

        uint8_t om = omega_arr[d];
        uint64_t bit = (om < 64) ? (1ull << om) : 0;
        if (om < 64 && !(used & bit)) {
            total += dfs(rem / d, d, used | bit);
        }
    }
    return total;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    ensure_limit(n);
    return dfs((uint64_t)n, 2, 0);
}
