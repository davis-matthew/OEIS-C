/*
 * A345792  Numbers that are the sum of eight cubes in exactly ten ways.
 * https://oeis.org/A345792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 161, u32 n <= 161, u64 n <= 161, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A345792 – Numbers that are the sum of eight cubes in exactly
    ten distinct unordered ways.
    This implementation builds the list by dynamic programming.
    It stores all found terms in a static array so that seq_t may be any
    integer type (including arbitrary‑precision types) without using
    realloc/malloc on seq_t objects.  The algorithm works for large
    n (≥1) and recomputes the table only when more terms are needed. */
    
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/*  seq_t and idx_t are defined by the build environment.  */

static seq_t  terms[200000];      /* preallocated buffer for all terms      */
static size_t term_count = 0;     /* number of terms actually stored        */
static idx_t  current_max_sum = 0;/* last sum processed in the table        */
static const size_t TERM_CAP = 200000;

/*  Build all terms up to the specified maximum sum.  The table is
    recomputed from scratch each time.  */
static void generate_terms_up_to(size_t max_sum)
{
    /*  Reset term list for new table.  */
    term_count = 0;
    current_max_sum = max_sum;

    /*  DP table: rows 0..8, columns 0..max_sum (inclusive).  */
    size_t rows   = 9;
    size_t cols   = max_sum + 1;
    size_t dp_len = rows * cols;
    uint8_t *dp = (uint8_t *)calloc(dp_len, 1);
    if (!dp) abort();                   /* allocation failure */

    /*  dp[0][0] = 1  */
    dp[0] = 1;

    /*  Enumerate cube values 1^3, 2^3, … up to max_sum.  */
    for (idx_t v = 1; ; ++v) {
        unsigned __int128 cube128 = (unsigned __int128)v * v * v;
        if (cube128 > max_sum) break;
        size_t cube = (size_t)cube128;

        /*  Update counts for k = 1..8 (using ascending k to allow
            repetitions and keep unordered collections unique).  */
        for (int k = 1; k <= 8; ++k) {
            size_t rowk   = k * cols;
            size_t rowkm1 = (k - 1) * cols;
            for (size_t s = cube; s <= max_sum; ++s) {
                uint8_t add = dp[rowkm1 + (s - cube)];
                if (!add) continue;
                uint8_t cur = dp[rowk + s];
                uint8_t nxt = cur + add;
                dp[rowk + s] = (nxt > 10) ? 11 : nxt;   /* cap at 11 to distinguish 10 */
            }
        }
    }

    /*  Collect all sums with exactly ten representations.  */
    for (size_t s = 1; s <= max_sum; ++s) {
        if (dp[8 * cols + s] == 10) {
            if (term_count >= TERM_CAP) abort();   /* safety check */
            terms[term_count++] = (seq_t)s;
        }
    }

    free(dp);
}

/*  Public function required by the framework.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n == 0 */

    /*  Generate terms until we have at least n of them.  */
    if (term_count < (size_t)n) {
        size_t max_sum = current_max_sum ? current_max_sum : 2000;
        while (term_count < (size_t)n) {
            generate_terms_up_to(max_sum);
            if (term_count >= (size_t)n) break;
            max_sum *= 2;           /* enlarge search bound */
        }
    }

    return terms[n - 1];
}
