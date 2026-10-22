/*
 * A348016  Record the number of terms with no proper divisors, then the
 *          number with one proper divisor, then two, three, etc., until
 *          reaching a zero term. After each zero term, repeat the count as
 *          before.
 * https://oeis.org/A348016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0)=0; thereafter let k be the current divisor index (starting
 *            0). a(n) = number of existing terms whose number of proper
 *            divisors equals k. After producing a(n), append it to the
 *            sequence list. If a(n)==0 then k reset to 0, else k++. This
 *            algorithm reproduces OEIS A348016
 * Range:     u8 n <= 815, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


#define MAX_N 20000            /* guarantee sufficient for n <= ~10000 */

static seq_t seq[MAX_N + 1];    /* stored sequence values */
static seq_t freq[MAX_N + 1];   /* freq[d] = #terms with d proper divisors */
static seq_t proper_cnt[MAX_N + 1]; /* proper_cnt[x] = #proper divisors of x */
static bool initialized = false;
static idx_t current_k = 0;     /* current divisor index k */
static idx_t seq_len = 0;       /* length of computed sequence */

static void init(void)
{
    if (initialized)
        return;

    /* compute proper divisor counts for 0..MAX_N */
    for (idx_t i = 1; i <= MAX_N; ++i)
        for (idx_t j = i; j <= MAX_N; j += i)
            proper_cnt[j] += 1;

    /* subtract self to obtain proper divisors for n >= 2 */
    for (idx_t i = 2; i <= MAX_N; ++i)
        proper_cnt[i] -= 1;

    proper_cnt[0] = 0;
    proper_cnt[1] = 0;

    /* freq array is zero‑initialised by static storage */
    initialized = true;
}

seq_t a(idx_t n)
{
    init();                  /* one‑time setup */

    /* extend sequence up to index n */
    for (idx_t i = seq_len; i <= n; ++i)
    {
        seq_t count = freq[current_k];          /* #terms with k proper divisors */
        seq[i] = count;                         /* store the term */

        /* update frequency of proper divisor count of the new term */
        idx_t div_index = (idx_t)proper_cnt[(idx_t)count];
        freq[div_index] += 1;

        /* advance k as specified */
        if (count == (seq_t)0)
            current_k = 0;
        else
            ++current_k;
    }

    seq_len = n + 1;
    return seq[n];
}
