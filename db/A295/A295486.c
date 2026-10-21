/*
 * A295486  Numbers that have exactly three representations as a sum of six
 *          nonnegative squares.
 * https://oeis.org/A295486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A295486: numbers with exactly three representations as a sum
 * of six nonnegative squares.
 *
 * The sequence is finite; all terms are ≤ 5408.  The routine precomputes
 * the set of numbers with exactly three unordered 6‑tuples (x1≥x2≥…≥x6≥0)
 * of squares summing to n and stores them in sorted order.  Subsequent
 * calls to a(n) simply index this table.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

#define MAX_N 5408

/* static state for memoised precomputation */
static seq_t  *s_values = NULL;
static size_t  s_count  = 0;
static bool    s_init   = false;

/* one–time precomputation */
static void init_sequence(void)
{
    if (s_init) return;
    s_init = true;

    /* array of counts of unordered representations */
    uint32_t *cnt = (uint32_t*)calloc(MAX_N + 1, sizeof(uint32_t));
    if (!cnt) return;          /* allocation failure – unlikely */

    uint32_t max_val = (uint32_t)(sqrt((double)MAX_N) + 0.5);

    for (uint32_t a = 0; a <= max_val; ++a) {
        uint32_t a2 = a * a;
        for (uint32_t b = 0; b <= a; ++b) {
            uint32_t ab = a2 + b * b;
            if (ab > MAX_N) continue;
            for (uint32_t c = 0; c <= b; ++c) {
                uint32_t abc = ab + c * c;
                if (abc > MAX_N) continue;
                for (uint32_t d = 0; d <= c; ++d) {
                    uint32_t abcd = abc + d * d;
                    if (abcd > MAX_N) continue;
                    for (uint32_t e = 0; e <= d; ++e) {
                        uint32_t abcde = abcd + e * e;
                        if (abcde > MAX_N) continue;
                        for (uint32_t f = 0; f <= e; ++f) {
                            uint32_t sum = abcde + f * f;
                            if (sum <= MAX_N) ++cnt[sum];
                        }
                    }
                }
            }
        }
    }

    /* collect all n with exactly three representations */
    size_t total = 0;
    for (idx_t i = 0; i <= MAX_N; ++i)
        if (cnt[i] == 3) ++total;

    if (total == 0) {
        free(cnt);
        return;
    }

    s_values = (seq_t*)malloc(total * sizeof(seq_t));
    if (!s_values) {
        free(cnt);
        return;
    }

    size_t pos = 0;
    for (idx_t i = 0; i <= MAX_N; ++i)
        if (cnt[i] == 3) s_values[pos++] = (seq_t)i;

    s_count = total;
    free(cnt);
}

/* required function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init_sequence();
    if (!s_values) return 0;
    if ((idx_t)(n - 1) < s_count) return s_values[n - 1];
    return 0;          /* out of range – behaviour unspecified */
}
