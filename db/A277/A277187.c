/*
 * A277187  Numbers k such that A001158(k) == 1 (mod k).
 * https://oeis.org/A277187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th positive integer k such that σ₃(k) ≡ 1 (mod k)
 *            (σ₃ is the sum of cubes of the divisors of k)
 * Range:     u8 n <= 65, u16 n <= 6610, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --- prime table ---------------------------------------------------- */
#define PRIME_LIMIT 200000   /* sufficient for n up to about 10000 */
static uint64_t primes[PRIME_LIMIT + 1];
static size_t   primes_cnt = 0;
static bool     primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    static bool sieve[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = false;
    primes_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            primes[primes_cnt++] = i;
            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve[j] = true;
            }
        }
    }
    primes_ready = true;
}

/* --- test whether k satisfies σ₃(k) ≡ 1 (mod k) ------------------- */
static bool satisfies(uint64_t k)
{
    init_primes();
    seq_t modk = (seq_t)k;                    /* modulus */
    seq_t sigma_mod = 1 % modk;               /* running product of σ₃(pᵉ) mod k */
    uint64_t rem = k;

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= rem; ++i) {
        uint64_t p = primes[i];
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            seq_t term = 1 % modk;            /* current power of p³ */
            seq_t s_mod = 1 % modk;           /* σ₃(pᵉ) mod k */
            seq_t p_cube = ((seq_t)p % modk * (seq_t)p % modk * (seq_t)p) % modk;
            for (int j = 1; j <= e; ++j) {
                term = (term * p_cube) % modk;
                s_mod += term;
                if (s_mod >= modk) s_mod -= modk;   /* keep s_mod < modk */
            }
            sigma_mod = (sigma_mod * s_mod) % modk;
        }
    }

    if (rem > 1) {                           /* remaining prime factor */
        seq_t p_cube = ((seq_t)rem % modk * (seq_t)rem % modk * (seq_t)rem) % modk;
        seq_t s_mod = 1 % modk + p_cube;
        if (s_mod >= modk) s_mod -= modk;
        sigma_mod = (sigma_mod * s_mod) % modk;
    }

    return (sigma_mod % modk) == (1 % modk);
}

/* --- dynamic array for sequence values ------------------------------ */
static seq_t *seq_vals = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t last_checked = 0;

static void ensure_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 16;
    while (newcap < needed) newcap <<= 1;
    seq_vals = (seq_t *)realloc(seq_vals, newcap * sizeof(seq_t));
    seq_cap = newcap;
}

/* --- public function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1; undefined for 0 */
    /* generate until we have at least n terms */
    while (seq_len < (size_t)n) {
        ++last_checked;
        if (satisfies(last_checked)) {
            ensure_cap(seq_len + 1);
            seq_vals[seq_len++] = (seq_t)last_checked;
        }
    }
    return seq_vals[n - 1];
}
