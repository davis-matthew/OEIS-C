/*
 * A355386  Position of first appearance of n in A355382, where A355382(m) =
 *          number of divisors d of m such that bigomega(d) = omega(m); or
 *          a(n) = -1 if n does not appear in A355382.
 * https://oeis.org/A355386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = position of first appearance of n in A355382 (A355382(m)
 *            = #divisors d of m with bigomega(d) = omega(m))
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   compute_A355382
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/compute_A355382.h"

static const idx_t MAX_M = 10000000;  /* Upper bound for m to search */

/* Lazy initialization data */
static bool initialized = false;
static seq_t *positions = NULL;   /* positions[n] = first m with A355382(m)=n, 0 means not found */
static size_t pos_size  = 0;      /* Current allocated size of positions array */

/* Builds SPF (smallest prime factor) array up to limit */
static void build_spf(uint32_t limit, uint32_t **spf_out)
{
    uint32_t *spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);
    for (uint32_t i = 0; i <= limit; ++i) spf[i] = i;
    uint32_t root = (uint32_t)sqrt((double)limit);
    for (uint32_t i = 2; i <= root; ++i) {
        if (spf[i] == i) {
            for (uint32_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
    *spf_out = spf;
}

/* Resizes positions array if needed */
static void ensure_positions_size(size_t needed)
{
    if (needed < pos_size) return;
    size_t new_size = needed * 2 + 10;
    seq_t *new_arr = (seq_t*)realloc(positions, new_size * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    /* Zero initialize the new portion */
    memset(new_arr + pos_size, 0, (new_size - pos_size) * sizeof(seq_t));
    positions = new_arr;
    pos_size = new_size;
}

/* Initializes the lookup by scanning m up to MAX_M */
static void init_lookup(void)
{
    uint32_t *spf = NULL;
    build_spf(MAX_M, &spf);

    positions = (seq_t*)malloc(10 * sizeof(seq_t)); /* initial small size */
    memset(positions, 0, 10 * sizeof(seq_t));
    pos_size = 10;

    for (uint32_t m = 1; m <= MAX_M; ++m) {
        int val = compute_A355382(m, spf);
        if (val <= 0) continue;      /* should not happen */
        if (val >= (int)pos_size) {
            /* enlarge */
            ensure_positions_size((size_t)val);
        }
        if (positions[val] == 0) {    /* first occurrence */
            positions[val] = (seq_t)m;
        }
    }

    free(spf);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_lookup();

    if (n == 0) return (seq_t)-1;
    if ((size_t)n >= pos_size) return (seq_t)-1;
    seq_t pos = positions[n];
    if (pos == 0) return (seq_t)-1;
    return pos;
}
