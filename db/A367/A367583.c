/*
 * A367583  Greatest element in row n of A367579 (multiset multiplicity
 *          kernel).
 * https://oeis.org/A367583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest prime index in the multiset multiplicity kernel
 *            of the multiset of prime indices of n
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- prime sieve (static) ---------- */
#define PRIME_LIMIT 200000
#define MAX_PRIMES  20000

static uint32_t primes[MAX_PRIMES];
static size_t   prime_cnt = 0;
static bool     primes_init = false;

static void init_primes(void)
{
    if (primes_init) return;
    static bool is_comp[PRIME_LIMIT + 1] = {0};
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_comp[j] = true;
        }
    }
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt < MAX_PRIMES)
                primes[prime_cnt++] = (uint32_t)i;
        }
    }
    primes_init = true;
}

/* binary search for 1‑based index of a prime */
static uint32_t prime_index(uint32_t p)
{
    init_primes();
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] >= p) hi = mid;
        else lo = mid + 1;
    }
    return (uint32_t)(lo + 1);  /* indices start at 1 */
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)0;

    init_primes();

    /* arrays for exponents up to 64 (more than enough) */
    uint32_t min_idx[65];
    bool     exp_seen[65];
    for (size_t i = 0; i < 65; ++i) exp_seen[i] = false;

    uint64_t m = (uint64_t)n;

    while (m > 1) {
        bool found = false;
        for (size_t i = 0; i < prime_cnt &&
             (uint64_t)primes[i] * (uint64_t)primes[i] <= m; ++i) {
            uint32_t p = primes[i];
            if (m % p == 0) {
                uint8_t e = 0;
                do {
                    m /= p;
                    ++e;
                } while (m % p == 0);
                uint32_t idx = prime_index(p);
                if (!exp_seen[e]) {
                    exp_seen[e] = true;
                    min_idx[e] = idx;
                } else if (idx < min_idx[e]) {
                    min_idx[e] = idx;
                }
                found = true;
                break;
            }
        }
        if (!found) { /* m is a prime > sqrt(original n) */
            uint32_t p  = (uint32_t)m;
            uint8_t  e  = 1;
            uint32_t idx = prime_index(p);
            if (!exp_seen[e]) {
                exp_seen[e] = true;
                min_idx[e] = idx;
            } else if (idx < min_idx[e]) {
                min_idx[e] = idx;
            }
            m = 1;
        }
    }

    seq_t result = (seq_t)0;
    for (size_t e = 1; e < 65; ++e) {
        if (exp_seen[e]) {
            seq_t val = (seq_t)min_idx[e];
            if (val > result) result = val;
        }
    }
    return result;
}
