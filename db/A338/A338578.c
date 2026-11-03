/*
 * A338578  Primes p such that (r-p)*(r-q) > r, where q and r are the next two
 *          primes.
 * https://oeis.org/A338578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   Prime p such that (r‑p)*(r‑q) > r, where q and r are the next
 *            two primes
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Grow a seq_t array to at least requested capacity. */
static void grow_array(seq_t **arr, size_t *len, size_t *cap, size_t req) {
    if (*cap >= req) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 8;
    while (new_cap < req) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) exit(EXIT_FAILURE);          /* Stop on allocation failure. */
    *arr = tmp;
    *cap = new_cap;
}

/* Ensure we have enough terms in the sequence to answer a query for n. */
static void ensure_sequence(idx_t n) {
    if (seq_len >= (size_t)n) return;          /* already enough */

    /* Initialise primality scan if not done already. */
    static bool init = false;
    static seq_t candidate = (seq_t)3;          /* candidate for next prime */

    if (!init) {
        grow_array(&primes, &primes_len, &primes_cap, 1);
        primes[0] = (seq_t)2;          /* the first prime */
        primes_len = 1;
        init = true;
    }

    while (seq_len < (size_t)n) {
        /* Find next prime. */
        while (!is_prime_big(candidate)) {
            candidate += (seq_t)2;
        }

        /* Record prime. */
        grow_array(&primes, &primes_len, &primes_cap, primes_len + 1);
        primes[primes_len++] = candidate;

        /* Evaluate condition when we have at least three primes. */
        if (primes_len >= 3) {
            seq_t p = primes[primes_len - 3];
            seq_t q = primes[primes_len - 2];
            seq_t r = primes[primes_len - 1];

            seq_t diff1 = r - p;
            seq_t diff2 = r - q;
            seq_t prod = diff1 * diff2;            /* safe in seq_t */

            if (prod > r) {
                grow_array(&seq, &seq_len, &seq_cap, seq_len + 1);
                seq[seq_len++] = p;
            }
        }

        /* Advance to next odd candidate. */
        candidate += (seq_t)2;
    }
}

/* External function required by the test harness. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;               /* undefined for index < 1 */
    ensure_sequence(n);
    return seq[n - 1];
}
