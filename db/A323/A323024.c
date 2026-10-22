/*
 * A323024  Numbers with exactly three distinct exponents in their prime
 *          factorization, or three distinct parts in their prime signature.
 * https://oeis.org/A323024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1742, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323024: Numbers with exactly three distinct exponents in
   their prime factorization.  The function returns the n‑th such
   number.  It lazily generates terms and caches them. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <sys/types.h>

static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;

static uint32_t *spf = NULL;
static size_t spf_size = 0;
static uint64_t last_checked = 0;

/* Build a smallest‑prime‑factor table up to 'limit' (inclusive).  The
   table is stored in a static array of uint32_t.  In a C++ build the
   same table is stored in the same format; the allocation is done with
   calloc and then cast to uint32_t*. */
static void build_spf(size_t limit)
{
    void *tmp = calloc(limit + 1, sizeof(uint32_t));
    spf = (uint32_t *)tmp;
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= limit)
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
        }
    }
    spf_size = limit + 1;
}

/* Ensure the spf table covers at least 'need'.  The table is doubled
   until it is large enough. */
static void ensure_spf(size_t need)
{
    if (spf == NULL || need >= spf_size) {
        size_t new_limit = spf_size == 0 ? 1024 : spf_size * 2;
        while (new_limit <= need) new_limit *= 2;
        build_spf(new_limit);
    }
}

/* Append a newly found value to the cache, growing it if necessary. */
static void cache_append(seq_t val)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* Generate terms until we have at least 'upto' members in the cache. */
static void generate_until(size_t upto)
{
    while (seq_len < upto) {
        /* Make sure spf is available for the next integer. */
        ensure_spf(last_checked + 1);
        ++last_checked;

        uint64_t x = last_checked;
        int seen[128] = {0};          /* exponent flags */
        int distinct = 0;

        while (x > 1) {
            uint32_t p = spf[x];
            int exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }

            if (!seen[exp]) {
                ++distinct;
                if (distinct > 3) break;
                seen[exp] = 1;
            }
        }

        if (distinct == 3) {
            cache_append((seq_t)last_checked);
        }
    }
}

/* Public sequence function.  Calls generate_until to ensure we have
   the requested term. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, guard */
    generate_until((size_t)n);
    return seq_cache[(size_t)n - 1];
}
