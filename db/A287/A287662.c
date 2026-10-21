/*
 * A287662  a(n) is the smallest positive integer not already in sequence such
 *          that a(1) + ... + a(n) is a prime power, with a(1) = 1.
 * https://oeis.org/A287662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A287662 – a(1)=1; for n>1, a(n) is the smallest unused positive
 *            integer such that the cumulative sum a(1)+…+a(n) is a prime
 *            power
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ---------- Prime sieve ---------- */
#define PRIME_LIMIT 200000   /* enough for all sums up to n≈10000 in 64‑bit */

static uint32_t primes[PRIME_LIMIT];
static size_t  prime_cnt  = 0;
static bool    primes_initted = false;

static void init_primes(void)
{
    bool *comp = (bool *)malloc(PRIME_LIMIT + 1);
    if (!comp) exit(1);
    memset(comp, 0, PRIME_LIMIT + 1);
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!comp[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    comp[j] = true;
            }
        }
    }
    free(comp);
    primes_initted = true;
}

/* ---------- Check if a 64‑bit integer is a prime power ---------- */
static bool is_prime_power_uint64(uint64_t n)
{
    if (n <= 1) return true;          /* 1 counts as a prime power */
    if (!primes_initted) init_primes();

    uint64_t p = 0;
    if (n % 2 == 0) {
        p = 2;
    } else {
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t pr = primes[i];
            uint64_t pr2 = (uint64_t)pr * pr;
            if (pr2 > n) break;
            if (n % pr == 0) { p = pr; break; }
        }
    }
    if (!p) return true;   /* n is prime */

    while (n % p == 0) n /= p;
    return n == 1;
}

/* ---------- Sequence storage ---------- */
static seq_t *seq = NULL;        /* array of terms */
static size_t seq_len  = 0;      /* number of terms stored */
static size_t seq_cap  = 0;      /* allocated capacity */

static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 256;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq = tmp;
    seq_cap = new_cap;
}

/* ---------- Used‑number bitset ---------- */
static uint8_t *used_bits = NULL;   /* bitset */
static size_t   used_bytes = 0;     /* allocated bytes */
static unsigned long long used_max = 0;   /* largest value ever set */

static void ensure_used_cap(unsigned long long val)
{
    size_t need = (val >> 3) + 1;
    if (need <= used_bytes) return;
    size_t new_cap = used_bytes ? used_bytes * 2 : 128;
    if (new_cap < need) new_cap = need;
    uint8_t *tmp = (uint8_t *)realloc(used_bits, new_cap);
    if (!tmp) exit(1);
    memset(tmp + used_bytes, 0, new_cap - used_bytes);
    used_bits = tmp;
    used_bytes = new_cap;
}

static void used_set(unsigned long long val)
{
    ensure_used_cap(val);
    used_bits[val >> 3] |= 1U << (val & 7U);
    if (val > used_max) used_max = val;
}

static bool used_test(unsigned long long val)
{
    if (val > used_max) return false;
    return (used_bits[val >> 3] >> (val & 7U)) & 1U;
}

/* ---------- Main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined, but harmless */

    /* Seed the first term if not yet done. */
    if (seq_len == 0) {
        ensure_seq_cap(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
        used_set(1);
    }

    /* Cumulative sum in 64‑bit for prime‑power test. */
    static uint64_t sum_uint64 = 1;
    static seq_t sum_seq = (seq_t)1;

    /* Initialise sums on the first use. */
    if (seq_len == 1 && n > 1) {
        sum_uint64 = 1;
        sum_seq   = (seq_t)1;
    }

    while (seq_len < (size_t)n) {
        /* Find next unused candidate starting from 1. */
        unsigned long long cand = 1;
        while (true) {
            while (used_test(cand)) ++cand;
            uint64_t new_sum = sum_uint64 + cand;
            if (is_prime_power_uint64(new_sum)) {
                /* Accept this candidate */
                ensure_seq_cap(seq_len + 1);
                seq[seq_len++] = (seq_t)cand;
                sum_seq += (seq_t)cand;
                sum_uint64 = new_sum;
                used_set(cand);
                break;                    /* next term */
            }
            ++cand;
        }
    }

    return seq[n - 1];
}
