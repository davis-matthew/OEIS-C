/*
 * A365545  Triangle read by rows where T(n,k) is the number of strict integer
 *          partitions of n with exactly k distinct non-subset-sums.
 * https://oeis.org/A365545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A365545:  Triangle T(n,k) of strict partitions of n
   with exactly k distinct non‑subset‑sums.  The sequence a(n) is the
   flattened triangle: a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1),
   a(3)=T(2,0), …  No closed formula is known; the values are
   obtained by enumerating all strict partitions of n.  The function
   is memoised so that successive calls remain fast. */

/* Required integer types are supplied by the build (seq_t, idx_t). */
#include <stdint.h>
#include <stdlib.h>

/* Number of 64‑bit words needed to hold subset sums up to n ≤ 200. */
#define WORDS 4

/* Forward declarations */
static void dfs_all(uint64_t remaining, uint64_t last, uint64_t cur[WORDS],
                    uint64_t r, seq_t *count);
static void compute_row(int r);
static void ensure_up_to(idx_t n);

/* Static storage for the flattened sequence */
static seq_t *flat      = NULL;   /* array of a(n) */
static size_t flat_len  = 0;      /* number of elements stored */
static size_t flat_cap  = 0;      /* allocated capacity of flat */

/* The last row for which we have already computed the triangle data */
static int current_row = -1;

/* Public function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return flat[n];
}

/* Make sure we have computed all rows up to the index n.  */
static void ensure_up_to(idx_t n)
{
    /* Compile time constant: n is unsigned; flat_len is size_t which is unsigned as well. */
    while (flat_len <= (size_t)n) {
        ++current_row;
        compute_row(current_row);
    }
}

/* Compute triangle row r (0‑based) and append its entries to flat. */
static void compute_row(int r)
{
    size_t len = (size_t)r + 1;
    seq_t *count = (seq_t *)malloc(len * sizeof(seq_t));
    if (!count) return;                /* allocation failed – keep function safe */
    for (size_t i = 0; i < len; ++i) count[i] = 0;

    if (r == 0) {
        count[0] = 1;                    /* the empty partition */
    } else {
        uint64_t cur[WORDS] = {1, 0, 0, 0};   /* bit 0 set: the empty sum */
        dfs_all((uint64_t)r, (uint64_t)r + 1, cur, (uint64_t)r, count);
    }

    /* Ensure there is enough space in flat. */
    if (flat_len + len > flat_cap) {
        size_t new_cap = flat_cap ? flat_cap * 2 : 256;
        while (new_cap < flat_len + len) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(flat, new_cap * sizeof(seq_t));
        if (!tmp) return;               /* allocation failed – stay safe */
        flat = tmp;
        flat_cap = new_cap;
    }

    /* Append to flat. */
    for (size_t i = 0; i < len; ++i) {
        flat[flat_len++] = count[i];
    }

    free(count);
}

/* Recursive enumeration of strict partitions of 'remaining', where the next part
   must be < 'last'.  'cur' holds the current subset‑sum bitset.  For each
   complete partition, count the missing sums 1…r and increment count[missing]. */
static void dfs_all(uint64_t remaining, uint64_t last, uint64_t cur[WORDS],
                    uint64_t r, seq_t *count)
{
    if (remaining == 0) {
        /* Count distinct non‑subset‑sums: missing positive sums <= r. */
        uint64_t missing = 0;
        for (uint64_t i = 1; i <= r; ++i) {
            uint64_t w = i >> 6;
            uint64_t b = i & 63;
            if ((cur[w] & (1ULL << b)) == 0) ++missing;
        }
        ++count[missing];
        return;
    }

    uint64_t max_part = (remaining < last) ? remaining : last - 1;
    for (uint64_t part = max_part; part > 0; --part) {
        uint64_t shift[WORDS] = {0, 0, 0, 0};
        uint64_t word_shift = part >> 6;
        uint64_t bit_shift  = part & 63;

        for (int w = 0; w < WORDS; ++w) {
            uint64_t val = cur[w];
            if (!val) continue;
            uint64_t idx = w + word_shift;
            if (idx < WORDS) {
                shift[idx] |= val << bit_shift;
                if (bit_shift && idx + 1 < WORDS) {
                    shift[idx + 1] |= val >> (64 - bit_shift);
                }
            }
        }

        uint64_t newBits[WORDS];
        for (int w = 0; w < WORDS; ++w) {
            newBits[w] = cur[w] | shift[w];
        }

        dfs_all(remaining - part, part, newBits, r, count);
    }
}
