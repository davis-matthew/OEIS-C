/*
 * A334736  Dimensions d such that the integer lattice Z^d does not contain
 *          the vertices of a regular d-simplex.
 * https://oeis.org/A334736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function enumerates integers d ≥ 2 such that the integer lattice
 * ℤ^d does not contain the vertices of a regular d‑simplex.
 *
 * The property is equivalent to:
 *   (1) d is even and d+1 is not a square, or
 *   (2) d ≡ 1 (mod 4) and d+1 is not representable as a sum of two squares.
 * The function generates these d lazily and stores them in a static cache
 * so that queries for any n ≥ 1 are answered in O(1) after an initial
 * O(n) construction.
 *
 * The implementation is C11 / C++17 compatible.  It uses only the
 * permitted operations and standard headers.  A lightweight sieve up to
 * 200 000 is used to factor numbers needed for the sum‑of‑two‑squares test.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- small integer sqrt (used for perfect‑square test) ---------- */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t x = n;
    idx_t y = (x + 1ULL) >> 1;
    while (y < x) {
        x = y;
        y = (x + (n / x)) >> 1;
    }
    return x;
}

/* ---------- prime sieve (up to a fixed limit) ---------- */
#define PRIME_LIMIT 200000   /* enough for all d up to n ≈ 10 000 */

static bool primes_built = false;
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

static void build_primes(void)
{
    if (primes_built) return;
    size_t limit = PRIME_LIMIT;
    bool *is_composite = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_composite) exit(1);

    size_t cap = limit / 10 + 100;
    primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!primes) exit(1);

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }
    }

    for (size_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            if (primes_cnt == cap) {
                cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    }

    free(is_composite);
    primes_built = true;
}

/* ---------- helper: perfect square check ---------- */
static bool is_square(idx_t x)
{
    idx_t r = isqrt_u64(x);
    return r * r == x;
}

/* ---------- helper: check whether a number is a sum of two squares ---------- */
static bool is_sum_of_two_squares(idx_t n)
{
    build_primes();

    idx_t temp = n;
    for (size_t i = 0; i < primes_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if ((p % 4U) == 3U && (exp & 1))
                return false;
        }
    }
    /* remaining prime factor (if > 1) */
    if (temp > 1) {
        if ((temp % 4U) == 3U)
            return false;
    }
    return true;
}

/* ---------- sequence cache ---------- */
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static idx_t next_d = 2;   /* next d to examine */

/* ---------- ensure enough capacity for the cache ---------- */
static void ensure_seq_capacity(void)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!newp) exit(1);
        if (seq) {
            memcpy(newp, seq, seq_len * sizeof(seq_t));
            free(seq);
        }
        seq = newp;
        seq_cap = new_cap;
    }
}

/* ---------- append a value to the cache ---------- */
static void append_value(seq_t val)
{
    ensure_seq_capacity();
    seq[seq_len++] = val;
}

/* ---------- main function required by the interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1 */
        return (seq_t)0;

    /* Generate terms until we have at least n of them */
    while (seq_len < n) {
        idx_t d = next_d;
        ++next_d;

        bool ok = false;
        if ((d & 1U) == 0) {                     /* d even */
            ok = !is_square(d + 1U);
        } else if ((d & 3U) == 1U) {             /* d ≡ 1 (mod 4) */
            ok = !is_sum_of_two_squares(d + 1U);
        }
        /* d ≡ 3 (mod 4) never satisfies the condition */

        if (ok)
            append_value((seq_t)d);
    }

    return seq[n - 1];
}
