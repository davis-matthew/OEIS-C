/*
 * A338921  a(0)=1, a(n) for n >= 1 is the number of distinct sums of two
 *          elements in [a(0), ..., a(n-1)], chosen without replacement.
 * https://oeis.org/A338921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A338921
 *  a(0) = 1
 *  a(n) = |{ a(i)+a(j) : 0 ≤ i < j < n }|   for n ≥ 1
 *
 *  The function builds the sequence lazily, caching values in a static array
 *  and maintaining a bitset of sums that have already appeared.
 *  Sums are stored in a dynamic array of 64‑bit words; each bit corresponds
 *  to one possible sum (sum = word*64 + bit_index).  Because all sums that
 *  appear for n ≤ 10000 fit into a 64‑bit value, this representation is
 *  sufficient for the required range.
 *
 *  The code compiles as both C11 and C++17 (the only C++ feature used is
 *  the C‑style cast from void* returned by realloc).  No external libraries
 *  are needed beyond the standard headers.
 */

#include <stdlib.h>   /* malloc, realloc, calloc, free */
#include <stdint.h>   /* uint64_t */

/* Static storage for the sequence and the set of distinct sums */
static seq_t  *g_vals      = NULL;   /* sequence values */
static size_t  g_vals_cap  = 0;      /* allocated capacity for g_vals */
static idx_t   g_computed  = 0;      /* next index that has not yet been computed */

static uint64_t *g_bits     = NULL;  /* dynamic bitset for sums */
static size_t   g_bits_cap  = 0;     /* number of 64‑bit words allocated */
static size_t   g_distinct  = 0;     /* current number of distinct sums */

/* Ensure the sequence array has capacity at least n+1 */
static void
ensure_vals(idx_t n)
{
    if (g_vals_cap <= (size_t)n) {
        size_t new_cap = g_vals_cap ? g_vals_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc((void *)g_vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        g_vals = tmp;
        g_vals_cap = new_cap;
    }
}

/* Ensure the bitset can store a sum equal to `sum` */
static void
ensure_bits(uint64_t sum)
{
    size_t word = sum >> 6;          /* word index */
    if (word >= g_bits_cap) {
        size_t new_cap = g_bits_cap ? g_bits_cap : 1;
        while (new_cap <= word) new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc((void *)g_bits, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);

        /* Zero any newly allocated words */
        for (size_t i = g_bits_cap; i < new_cap; ++i) tmp[i] = 0;

        g_bits = tmp;
        g_bits_cap = new_cap;
    }
}

/* Insert a sum into the bitset.  Return 1 if the sum was new, 0 otherwise. */
static int
add_sum(uint64_t sum)
{
    ensure_bits(sum);
    size_t word = sum >> 6;
    uint64_t mask = 1ULL << (sum & 63);
    if (g_bits[word] & mask) return 0;          /* already present */
    g_bits[word] |= mask;
    return 1;                                   /* new sum added */
}

/* -------------  public function  --------------------------------- */

seq_t a(idx_t n)
{
    /* Lazy initialization: set a(0) = 1 and prepare structures */
    if (!g_vals) {
        g_vals_cap   = 1;
        g_vals       = (seq_t *)calloc(g_vals_cap, sizeof(seq_t));
        g_vals[0]    = (seq_t)1;          /* a(0) = 1 */
        g_computed   = 1;                  /* next index to compute */
        g_bits_cap   = 0;
        g_bits       = NULL;
        g_distinct   = 0;
    }

    /* Compute terms up to n on demand */
    for (idx_t k = g_computed; k <= n; ++k) {
        seq_t val;
        if (k == 0)
            val = (seq_t)1;          /* safety; never reached after init */
        else
            val = (seq_t)g_distinct; /* a(k) = current number of distinct sums */

        ensure_vals(k);
        g_vals[k] = val;

        /* Add all new pair sums that involve the new element */
        for (idx_t i = 0; i < k; ++i) {
            uint64_t s = (uint64_t)g_vals[i] + (uint64_t)val;
            if (add_sum(s))
                ++g_distinct;
        }
    }
    g_computed = n + 1;   /* all indices up to n are now computed */

    return g_vals[n];
}
