/*
 * A244180  Number of length n+4 0..5 arrays with no five consecutive elements
 *          with pattern ababa or abbba (possibly a=b) and new values 0..5
 *          introduced in 0..5 order.
 * https://oeis.org/A244180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+4 0..5 arrays with no five consecutive
 *            elements with pattern ababa or abbba, and new values 0..5
 *            introduced in ascending order
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define STATE_DIGITS  7                   /* 0..5 plus sentinel 6 */
#define STATE_COUNT   (STATE_DIGITS*STATE_DIGITS*STATE_DIGITS*STATE_DIGITS) /* 2401 */
#define MAX_N         20000                /* sufficient for requested range */

/* ----------  transition table  ---------- */
static int16_t trans[6][STATE_COUNT][7];
static bool trans_ready = false;
static const int START_IDX = 2400;         /* encode(6,6,6,6) */

/* encoding of a 4‑tuple (d0,d1,d2,d3) */
static inline int encode(int d0,int d1,int d2,int d3)
{
    return (((d0*7)+d1)*7+d2)*7+d3;
}

/* precompute all transitions */
static void init_transitions(void)
{
    if (trans_ready) return;
    for (int m=0;m<6;++m)
        for (int s=0;s<STATE_COUNT;++s)
            for (int e=0;e<7;++e)
                trans[m][s][e] = -1;

    for (int m=0;m<6;++m) {
        for (int s=0;s<STATE_COUNT;++s) {
            /* decode the state */
            int x = s;
            int d3 = x % 7, x2 = x / 7;
            int d2 = x2 % 7, x1 = x2 / 7;
            int d1 = x1 % 7, d0 = x1 / 7;   /* (d0,d1,d2,d3) */

            for (int e=0;e<7;++e) {
                if (e > 5) continue;               /* no digit 6+ */
                /* allowed new digit?  e <= m or e == m+1 (if e <= 5) */
                if (e > m && !(e == m+1)) continue;
                /* disallow first step with digit 1 */
                if (m==0 && s==START_IDX && e==1) continue;

                /* pattern check only if we already have four real digits */
                if (d0 != 6) {
                    if ( (d0==d2 && d1==d3 && e==d0) ||
                         (d0==e   && d1==d2 && d1==d3) )
                        continue;                /* forbidden */
                }

                /* form next state */
                int nd0 = d1, nd1 = d2, nd2 = d3, nd3 = e;
                int ns = encode(nd0,nd1,nd2,nd3);
                trans[m][s][e] = (int16_t)ns;
            }
        }
    }
    trans_ready = true;
}

/* ----------  DP tables  ---------- */
static seq_t dp_cur[6][STATE_COUNT];
static seq_t dp_next[6][STATE_COUNT];
static seq_t (*dp_cur_ptr)[STATE_COUNT] = dp_cur;
static seq_t (*dp_next_ptr)[STATE_COUNT] = dp_next;
static idx_t cur_len = 0;                  /* length represented by dp_cur_ptr */

/* ----------  cache for a(n) ---------- */
static seq_t a_cache[MAX_N+1];
static idx_t cache_max = 0;

/* initialise DP the first time */
static bool dp_inited = false;
static void init_dp(void)
{
    init_transitions();
    for (int m=0;m<6;++m)
        for (int s=0;s<STATE_COUNT;++s)
            dp_cur_ptr[m][s] = 0;
    dp_cur_ptr[0][START_IDX] = 1;         /* empty word, no digits yet */
    cur_len = 0;
    cache_max = 0;
    dp_inited = true;
}

/* extend DP up to length n+4 and fill cache up to n */
static void compute_to(idx_t n)
{
    if (n == 0) return;                  /* offset is 1 */
    if (!dp_inited) init_dp();

    idx_t target_len = n + 4;
    while (cur_len < target_len) {
        /* clear dp_next */
        for (int m=0;m<6;++m)
            for (int s=0;s<STATE_COUNT;++s)
                dp_next_ptr[m][s] = 0;

        /* transitions */
        for (int m=0;m<6;++m) {
            for (int s=0;s<STATE_COUNT;++s) {
                seq_t cnt = dp_cur_ptr[m][s];
                if (cnt == 0) continue;
                for (int e=0;e<7;++e) {
                    int16_t nx = trans[m][s][e];
                    if (nx < 0) continue;
                    int nm = (e == m+1) ? e : m;
                    dp_next_ptr[nm][(int)nx] += cnt;
                }
            }
        }

        /* swap buffers */
        seq_t (*tmp)[STATE_COUNT] = dp_cur_ptr;
        dp_cur_ptr = dp_next_ptr;
        dp_next_ptr = tmp;

        ++cur_len;

        /* store a(k) for k = cur_len-4 when length >=5 */
        if (cur_len >= 5) {
            idx_t k = cur_len - 4;
            seq_t total = 0;
            for (int m=0;m<6;++m)
                for (int s=0;s<STATE_COUNT;++s)
                    total += dp_cur_ptr[m][s];
            a_cache[k] = total;
            if (k > cache_max) cache_max = k;
        }
    }
}

/* ----------  required function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n > MAX_N) return (seq_t)0;      /* safety for out‑of‑range */
    compute_to(n);
    return a_cache[n];
}
