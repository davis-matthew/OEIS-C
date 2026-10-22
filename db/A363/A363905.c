/*
 * A363905  Numbers whose square and cube taken together contain each decimal
 *          digit.
 * https://oeis.org/A363905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 809, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   digit_counts, seq_alloc, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A363905
    Formula: For each integer n ≥ 1, include n if the concatenation of
    n² and n³ contains every decimal digit 0‑9 at least once.
    The sequence is generated lazily in ascending order and cached.
*/

#include <stdbool.h>

/*  Include the utilities that provide the needed helpers  */
#include "../utils/seq_mul.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/digit_counts.h"

/*  Cache of found sequence values  */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 1;   /*  candidate to test next  */
static bool cache_initialized = false;

/*  Initialise the cache on first use  */
static void init_cache(void)
{
    if (!cache_initialized) {
        cache_cap = 16;
        cache = seq_alloc(cache_cap);
        cache_len = 0;
        cache_initialized = true;
    }
}

/*  Ensure the cache has room for at least one more element  */
static void ensure_cache_room(void)
{
    if (cache_len == cache_cap) {
        cache_cap *= 2;
        cache = seq_realloc(cache, cache_len, cache_cap);
    }
}

/*  Test whether n² and n³ together contain all digits 0‑9 at least once  */
static bool has_all_digits(seq_t n)
{
    seq_t sq  = seq_mul(n, n);
    seq_t cu  = seq_mul(n, sq);

    unsigned char cnt[10] = {0};
    unsigned char tmp[10];

    digit_counts(sq, cnt);   /*  count digits of the square  */
    digit_counts(cu, tmp);   /*  count digits of the cube    */
    for (int i = 0; i < 10; ++i) cnt[i] += tmp[i];

    for (int i = 0; i < 10; ++i) {
        if (cnt[i] == 0) return false;
    }
    return true;
}

/*  Generate the sequence up to at least index target (1‑based)  */
static void generate_up_to(idx_t target)
{
    init_cache();

    while (cache_len < (size_t)target) {
        seq_t cand = next_candidate++;

        if (has_all_digits(cand)) {
            ensure_cache_room();
            cache[cache_len++] = cand;
        }
    }
}

/*  Public function required by the grader  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /*  behaviour below offset is irrelevant  */
    generate_up_to(n);
    return cache[n - 1];
}
