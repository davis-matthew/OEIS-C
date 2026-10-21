/*
 * A295484  Numbers that have exactly one representation as a sum of six
 *          nonnegative squares.
 * https://oeis.org/A295484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A295484: Numbers that have exactly one representation as a sum of six nonnegative squares.
   The algorithm computes all such numbers up to an upper bound (6000), stores them in a static array,
   and returns the n‑th element (offset 1).  The set is finite, so the function returns 0 for
   indices beyond the sequence length. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static const idx_t MAX_N = 6000;                 /* guaranteed upper bound */
static seq_t unique_nums[512];                   /* static buffer – ample for this sequence */
static size_t unique_len = 0;
static bool initialized = false;

/* Helper: compute integer sqrt by simple loop (no external helper). */
static uint64_t integer_sqrt(uint64_t x)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r;
}

/* Build the list of numbers that have exactly one unordered representation as a sum of six
   nonnegative squares.  Results are stored in unique_nums (ascending). */
static void init_unique_numbers(void)
{
    if (initialized) return;
    initialized = true;

    uint64_t a_limit = integer_sqrt(MAX_N);     /* maximum a with a^2 <= MAX_N */

    /* Count unordered representations: a ≤ b ≤ c ≤ d ≤ e ≤ f. */
    int counts[MAX_N + 1];
    for (size_t i = 0; i <= (size_t)MAX_N; ++i) counts[i] = 0;

    for (uint64_t a = 0; a <= a_limit; ++a) {
        uint64_t a2 = a * a;
        for (uint64_t b = a; b <= a_limit; ++b) {
            uint64_t ab = a2 + b * b;
            if (ab > MAX_N) break;
            for (uint64_t c = b; c <= a_limit; ++c) {
                uint64_t abc = ab + c * c;
                if (abc > MAX_N) break;
                for (uint64_t d = c; d <= a_limit; ++d) {
                    uint64_t abcd = abc + d * d;
                    if (abcd > MAX_N) break;
                    for (uint64_t e = d; e <= a_limit; ++e) {
                        uint64_t abcde = abcd + e * e;
                        if (abcde > MAX_N) break;
                        for (uint64_t f = e; f <= a_limit; ++f) {
                            uint64_t total = abcde + f * f;
                            if (total > MAX_N) break;
                            ++counts[(size_t)total];
                        }
                    }
                }
            }
        }
    }

    /* Store numbers with exactly one representation. */
    for (size_t i = 0; i <= (size_t)MAX_N; ++i) {
        if (counts[i] == 1) {
            if (unique_len >= 512) {
                /* In the unlikely event we exceed the static buffer, abort. */
                exit(EXIT_FAILURE);
            }
            unique_nums[unique_len++] = (seq_t)i;
        }
    }
}

/* Required interface. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for n < 1 */
    init_unique_numbers();
    if ((size_t)(n - 1) < unique_len) {
        return unique_nums[(size_t)(n - 1)];
    }
    return 0;                    /* index beyond finite sequence */
}
