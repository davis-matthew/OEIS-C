/*
 * A250368  Number of length n+6 0..3 arrays with every seven consecutive
 *          terms having the maximum of some three terms equal to the minimum
 *          of the remaining four terms.
 * https://oeis.org/A250368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of length n+6 arrays over {0,1,2,3} such that every seven
 *            consecutive terms contain a 3‑tuple whose maximum equals the
 *            minimum of the remaining four terms
 * Range:     u8 none, u16 n <= 2, u32 n <= 13, u64 n <= 34, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define STATE_BITS 12                     /* 4^6 = 4096 states   */
#define STATE_SIZE (1U << STATE_BITS)

#define WINDOW_BITS 14                    /* 4^7 = 16384 windows */
#define WINDOW_SIZE (1U << WINDOW_BITS)

#define MAX_TOTALS 20000                  /* enough for n ≤ 10000 */

static seq_t totals[MAX_TOTALS];          /* totals[L] = #arrays of length L */
static seq_t dp_cur[STATE_SIZE];          /* dp for current length (last 6 digits) */
static seq_t dp_nxt[STATE_SIZE];          /* temporary buffer for next length */

static seq_t *dp_cur_ptr = dp_cur;        /* pointers for swapping */
static seq_t *dp_nxt_ptr = dp_nxt;

static bool valid[WINDOW_SIZE];           /* precomputed window validity */
static bool init_done = false;
static idx_t max_len = 0;                 /* maximum length computed so far */

/* Precompute validity of all 7‑digit windows (0..3 each). */
static void precompute_valid(void)
{
    uint32_t masks[35];
    unsigned mcnt = 0;
    for (unsigned a = 0; a < 7; ++a)
        for (unsigned b = a + 1; b < 7; ++b)
            for (unsigned c = b + 1; c < 7; ++c)
                masks[mcnt++] = (1U << a) | (1U << b) | (1U << c);

    for (uint32_t w = 0; w < WINDOW_SIZE; ++w) {
        uint8_t d[7];
        uint32_t tmp = w;
        /* decode: d[0] is the most recent digit (least significant bits) */
        for (unsigned pos = 0; pos < 7; ++pos) {
            d[pos] = tmp & 3U;
            tmp >>= 2;
        }
        bool ok = false;
        for (unsigned m = 0; m < 35; ++m) {
            uint8_t mx = 0, mn = 3;
            uint32_t mask = masks[m];
            for (unsigned pos = 0; pos < 7; ++pos) {
                if (mask & (1U << pos))
                    mx = (d[pos] > mx) ? d[pos] : mx;
                else
                    mn = (d[pos] < mn) ? d[pos] : mn;
            }
            if (mx == mn) {
                ok = true;
                break;
            }
        }
        valid[w] = ok;
    }
}

/* One‑time initialization. */
static void init_once(void)
{
    if (init_done) return;

    /* totals for lengths 0..6 : all sequences allowed */
    totals[0] = 1;
    for (idx_t i = 1; i <= 6; ++i)
        totals[i] = totals[i - 1] * (seq_t)4;

    /* dp for length 6 : all 6‑tuples allowed */
    for (uint32_t s = 0; s < STATE_SIZE; ++s)
        dp_cur[s] = 1;

    precompute_valid();

    max_len = 6;
    init_done = true;
}

/* Extend precomputation up to length L (inclusive). */
static void ensure_length(idx_t L)
{
    if (L <= max_len) return;
    if (L >= MAX_TOTALS) {
        /* In normal usage with n ≤ 10000 L ≤ 10006 < MAX_TOTALS. */
        return;
    }

    for (size_t len = (size_t)(max_len + 1); len <= (size_t)L; ++len) {
        if (len <= 6) {
            totals[len] = totals[len - 1] * (seq_t)4;
            continue;
        }

        /* zero next buffer */
        memset(dp_nxt_ptr, 0, STATE_SIZE * sizeof(seq_t));

        /* extend each state by one new digit */
        for (uint32_t s = 0; s < STATE_SIZE; ++s) {
            seq_t val = dp_cur_ptr[s];
            if (!val) continue;
            for (int d = 0; d < 4; ++d) {
                uint32_t w = ((s << 2) | (uint32_t)d) & (WINDOW_SIZE - 1);
                if (!valid[w]) continue;
                uint32_t ns = ((s << 2) | (uint32_t)d) & (STATE_SIZE - 1);
                dp_nxt_ptr[ns] += val;
            }
        }

        /* swap dp buffers */
        seq_t *tmp = dp_cur_ptr;
        dp_cur_ptr = dp_nxt_ptr;
        dp_nxt_ptr = tmp;

        /* sum to obtain total for this length */
        seq_t sum = 0;
        for (uint32_t s = 0; s < STATE_SIZE; ++s)
            sum += dp_cur_ptr[s];
        totals[len] = sum;
    }

    max_len = L;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* behaviour below offset does not matter */

    init_once();

    idx_t L = n + 6;          /* actual array length */
    ensure_length(L);

    return totals[L];
}
