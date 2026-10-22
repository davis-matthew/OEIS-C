/*
 * A354188  a(n) = 1 if phi(A267099(n)) is equal to phi(n), and 0 otherwise.
 *          Here A267099 is fully multiplicative involution swapping the
 *          positions of 4k+1 and 4k+3 primes, and phi is Euler totient
 *          function.
 * https://oeis.org/A354188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9516, u16 n <= 9516, u32 n <= 9516, u64 n <= 9516,
 *            u128 n <= 9516, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A354188: a(n) = 1 if phi(A267099(n)) == phi(n), 0 otherwise. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_PRIME 20000           /* enough for n up to ~10 000 */

/* static data */
static uint32_t primes_arr[3000];
static size_t primes_cnt = 0;     /* number of primes <= MAX_PRIME */

static uint32_t primes1[1700];
static uint32_t primes3[1700];
static size_t cnt1 = 0, cnt3 = 0;

static uint64_t prime_map[MAX_PRIME + 1] = {0};

/* ------------------------------------------------------------------ */
/* initialization routine: sieve primes and build the prime involution */
/* ------------------------------------------------------------------ */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    bool is_comp[MAX_PRIME + 1];
    for (size_t i = 0; i <= MAX_PRIME; ++i) is_comp[i] = false;
    for (size_t i = 2; i * i <= MAX_PRIME; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= MAX_PRIME; j += i) is_comp[j] = true;
        }
    }
    for (size_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_comp[i]) primes_arr[primes_cnt++] = (uint32_t)i;
    }

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes_arr[i];
        if (p % 4 == 1) {
            primes1[cnt1++] = p;
        } else if (p % 4 == 3) {
            primes3[cnt3++] = p;
        }
    }

    size_t pairs = cnt1 < cnt3 ? cnt1 : cnt3;
    for (size_t i = 0; i < pairs; ++i) {
        prime_map[primes1[i]] = primes3[i];
        prime_map[primes3[i]] = primes1[i];
    }
    prime_map[2] = 2;     /* 2 is fixed by the involution */
}

/* ------------------------------------------------------------------ */
/* helper: map a prime via the involution (fails gracefully for >MAX)  */
/* ------------------------------------------------------------------ */
static uint64_t map_prime(uint64_t p)
{
    if (p <= MAX_PRIME) {
        uint64_t mp = prime_map[(size_t)p];
        if (mp) return mp;
    }
    if (p == 2) return 2;
    /* for primes > MAX_PRIME we default to itself – should not occur for tested range */
    return p;
}

/* ------------------------------------------------------------------ */
/* main function implementing the sequence                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined before offset */
    init_primes();

    idx_t tmp = n;
    seq_t phi_n   = (seq_t)1;
    seq_t phi_f_n = (seq_t)1;

    /* factorization by trial division using precomputed primes */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes_arr[i];
        if ((uint64_t)p * p > (uint64_t)tmp) break;
        if (tmp % p == 0) {
            size_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }

            /* compute p^exp as seq_t */
            seq_t p_pow = (seq_t)1;
            for (size_t k = 0; k < exp; ++k) p_pow *= (seq_t)p;
            /* phi part for this prime */
            seq_t part_n = (seq_t)(p - 1) * (p_pow / (seq_t)p);
            phi_n *= part_n;

            /* mapped prime */
            uint64_t f = map_prime(p);
            /* compute f^exp */
            seq_t f_pow = (seq_t)1;
            for (size_t k = 0; k < exp; ++k) f_pow *= (seq_t)f;
            seq_t part_f = (seq_t)(f - 1) * (f_pow / (seq_t)f);
            phi_f_n *= part_f;
        }
    }

    /* remaining prime factor (if any) */
    if (tmp > 1) {
        uint64_t p = (uint64_t)tmp;
        seq_t part_n = (seq_t)(p - 1);
        phi_n *= part_n;

        uint64_t f = map_prime(p);
        seq_t part_f = (seq_t)(f - 1);
        phi_f_n *= part_f;
    }

    return (phi_n == phi_f_n) ? (seq_t)1 : (seq_t)0;
}
