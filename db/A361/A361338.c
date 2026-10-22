/*
 * A361338  Number of different single-digit numbers that can be reached from
 *          n by any permissible sequence of split-and-multiply operations.
 * https://oeis.org/A361338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct single‑digit numbers reachable from n
 *            * by any sequence of split‑and‑multiply operations
 * Range:     u8 n <= 9683, u16 n <= 9683, u32 n <= 9683, u64 n <= 9683,
 *            u128 n <= 9683, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Pre‑computed powers of 10 up to 10^19 (maximum needed for 64‑bit numbers). */
static const unsigned long long pow10[20] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL
};

static unsigned *mask_bits = NULL;   /* bitmask of reachable digits for each value */
static bool     *computed  = NULL;   /* flag whether a value has been computed */
static size_t   cache_size = 0;      /* current capacity of the arrays */

/* Grow memoisation arrays so that index n is valid. */
static void grow_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_size)
        return;

    size_t new_size = cache_size ? cache_size : 64;
    while ((size_t)n + 1 > new_size)
        new_size <<= 1;

    unsigned *new_masks = (unsigned *)realloc(mask_bits, new_size * sizeof(unsigned));
    bool     *new_flags = (bool *)realloc(computed,  new_size * sizeof(bool));
    if (!new_masks || !new_flags) {
        exit(EXIT_FAILURE);  /* Cannot continue without memory */
    }

    for (size_t i = cache_size; i < new_size; ++i)
        new_flags[i] = false;

    mask_bits = new_masks;
    computed  = new_flags;
    cache_size = new_size;
}

/* Return the number of decimal digits of x (x >= 0). */
static inline int num_digits(uint64_t x)
{
    int d = 1;
    while (x >= 10ULL) {
        x /= 10ULL;
        ++d;
    }
    return d;
}

/* Recursive memoisation of the reachable‑digit mask for value v. */
static void ensure_value(idx_t v)
{
    grow_cache(v);

    if (computed[v])
        return;

    /* Base case: single‑digit numbers. */
    if (v <= 9u) {
        mask_bits[v] = (1u << (unsigned)v);
        computed[v]  = true;
        return;
    }

    unsigned result_mask = 0u;
    int len = num_digits(v);

    for (int pos = 1; pos < len; ++pos) {
        int lenR = len - pos;             /* length of the right part */
        unsigned long long divisor = pow10[lenR];
        idx_t left  = v / divisor;
        idx_t right = v % divisor;

        /* Disallow right part with leading zeros unless it is exactly 0. */
        if (lenR > 1 && right < pow10[lenR - 1])
            continue;

        idx_t prod = left * right;
        ensure_value(prod);
        result_mask |= mask_bits[prod];
    }

    mask_bits[v] = result_mask;
    computed[v]  = true;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    ensure_value(n);

    unsigned mask = mask_bits[n];
    int count = 0;
    while (mask) {
        count += (mask & 1u);
        mask >>= 1;
    }
    return (seq_t)count;
}
