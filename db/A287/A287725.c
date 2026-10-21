/*
 * A287725  {0->1, 1->011}-transform of the infinite Fibonacci word A003849.
 * https://oeis.org/A287725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth bit of the word obtained by replacing each 0 with “1”
 *            and each 1 with “011” in the infinite Fibonacci word A003849
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Helpers for the Fibonacci word -------------------------------- */

static uint64_t fib_nums[128];
static size_t fib_count = 0;

/* Ensure fib_nums holds Fibonacci numbers (1,2,3,5,…) up to at least n. */
static void ensure_fib_upto(uint64_t n)
{
    if (fib_count == 0) {
        fib_nums[0] = 1;
        fib_nums[1] = 2;
        fib_count = 2;
    }
    while (fib_nums[fib_count - 1] <= n) {
        fib_nums[fib_count] = fib_nums[fib_count - 1] + fib_nums[fib_count - 2];
        ++fib_count;
    }
}

/* ---------- State for generated bits and prefix sums ---------------------- */

static uint8_t  *fib_bits = NULL;   /* original Fibonacci word bits */
static size_t   fib_cap  = 0;

static uint64_t *pref_sum = NULL;   /* cumulative length after 0→1 / 1→011 */
static size_t   pref_cap  = 0;
static size_t   pref_len  = 0;

/* Grow the array that holds the Fibonacci word bits. */
static void grow_fib_bits(void)
{
    size_t newcap = fib_cap ? fib_cap << 1 : 16;
    uint8_t *tmp = (uint8_t *)realloc(fib_bits, newcap * sizeof(uint8_t));
    if (!tmp) abort();
    fib_bits = tmp;
    fib_cap  = newcap;
}

/* Grow the array that holds the cumulative transformed lengths. */
static void grow_pref_sum(void)
{
    size_t newcap = pref_cap ? pref_cap << 1 : 16;
    uint64_t *tmp = (uint64_t *)realloc(pref_sum, newcap * sizeof(uint64_t));
    if (!tmp) abort();
    pref_sum = tmp;
    pref_cap = newcap;
}

/* Compute the next original Fibonacci bit and update the prefix sums. */
static void compute_next_bit(void)
{
    size_t i = pref_len;            /* index of the new bit (0‑based) */
    bool bit;

    if (i == 0) {
        bit = false;                /* F0 = 0 */
    } else if (i == 1) {
        bit = true;                 /* F1 = 1 */
    } else {
        ensure_fib_upto((uint64_t)i);
        size_t k = fib_count - 1;
        while (fib_nums[k] > (uint64_t)i) --k;
        bit = fib_bits[i - fib_nums[k]];
    }

    if (pref_len == fib_cap)  grow_fib_bits();
    if (pref_len == pref_cap) grow_pref_sum();

    fib_bits[pref_len] = bit ? 1 : 0;

    uint64_t len = bit ? 3 : 1;               /* 0→“1”, 1→“011” */
    uint64_t prev = (pref_len == 0) ? 0 : pref_sum[pref_len - 1];
    pref_sum[pref_len] = prev + len;
    ++pref_len;
}

/* Ensure the table of cumulative lengths reaches at least target (0‑based). */
static void ensure_pref_sum(uint64_t target)
{
    while (pref_len == 0 || pref_sum[pref_len - 1] <= target)
        compute_next_bit();
}

/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for n=0, but requested to return 0 */

    uint64_t idx = (uint64_t)n - 1;          /* convert to 0‑based index */
    ensure_pref_sum(idx);

    size_t i = pref_len - 1;
    uint64_t prev = (i == 0) ? 0 : pref_sum[i - 1];
    uint64_t pos  = idx - prev;              /* position inside the block */

    if (fib_bits[i] == 0)                    /* original bit 0 → block “1” */
        return (seq_t)1;

    /* original bit 1 → block “011” */
    return (pos == 0) ? (seq_t)0 : (seq_t)1;
}
