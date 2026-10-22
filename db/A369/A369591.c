/*
 * A369591  a(n) = number of Yamanouchi words over the alphabet {1, 2, 3} of
 *          length n that start with 3.
 * https://oeis.org/A369591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = number of Yamanouchi words over alphabet {1,2,3} of
 *            length n that start with 3. The implementation counts words
 *            satisfying that for every suffix the number of 1's is at least
 *            the number of 2's and the number of 2's is at least the number
 *            of 3's. It builds the word from the right end, incre...
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 25, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define CAP 32768   /* power of two, enough for all hash entries */

static int h_keys[CAP];
static seq_t h_vals[CAP];
static unsigned char h_used[CAP];
static int h_usedIdx[CAP];
static int h_usedCnt;

/* helper to add a state to the hash map */
static void add_state(int a, int b, seq_t val, int base)
{
    int key = a * base + b;
    int h = key & (CAP - 1);
    while (h_used[h]) {
        if (h_keys[h] == key) {
            h_vals[h] += val;
            return;
        }
        h = (h + 1) & (CAP - 1);
    }
    h_keys[h] = key;
    h_vals[h] = val;
    h_used[h] = 1;
    h_usedIdx[h_usedCnt++] = h;
}

/* state arrays */
static int cur_a[10001];
static int cur_b[10001];
static seq_t cur_val[10001];
static int cur_sz;

static int nxt_a[10001];
static int nxt_b[10001];
static seq_t nxt_val[10001];
static int nxt_sz;

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1 */
    idx_t N = n; /* work with native integer for array bounds */
    if (N == 0) return (seq_t)0; /* safety, though offset starts at 1 */

    /* reset hash map */
    for (int i = 0; i < CAP; ++i) h_used[i] = 0;
    h_usedCnt = 0;

    /* initial state: no letters processed yet */
    cur_sz = 1;
    cur_a[0] = 0;
    cur_b[0] = 0;
    cur_val[0] = (seq_t)1;

    int base = (int)(N + 1); /* for key encoding: key = a*base + b */

    /* perform n-1 steps building the word from right to left */
    for (idx_t step = 0; step < N - 1; ++step) {
        /* insert transitions into hash map */
        for (int i = 0; i < cur_sz; ++i) {
            int a = cur_a[i];
            int b = cur_b[i];
            seq_t val = cur_val[i];

            /* add letter 1: always allowed */
            add_state(a + 1, b, val, base);

            /* add letter 2: requires a >= 1 */
            if (a >= 1)
                add_state(a - 1, b + 1, val, base);

            /* add letter 3: requires b >= 1 */
            if (b >= 1)
                add_state(a, b - 1, val, base);
        }

        /* transfer hash map contents to next state arrays */
        nxt_sz = 0;
        for (int i = 0; i < h_usedCnt; ++i) {
            int idx = h_usedIdx[i];
            int key = h_keys[idx];
            int a = key / base;
            int b = key % base;
            seq_t val = h_vals[idx];

            nxt_a[nxt_sz] = a;
            nxt_b[nxt_sz] = b;
            nxt_val[nxt_sz] = val;
            ++nxt_sz;

            /* reset hash entry for the next iteration */
            h_keys[idx] = 0;
            h_used[idx] = 0;
        }
        h_usedCnt = 0;

        /* swap current and next arrays */
        for (int i = 0; i < nxt_sz; ++i) {
            cur_a[i] = nxt_a[i];
            cur_b[i] = nxt_b[i];
            cur_val[i] = nxt_val[i];
        }
        cur_sz = nxt_sz;
    }

    /* after building n-1 letters (positions 2..n), add the final leftmost letter 3
       which is allowed only when b >= 1 */
    seq_t ans = 0;
    for (int i = 0; i < cur_sz; ++i) {
        if (cur_b[i] >= 1)
            ans += cur_val[i];
    }
    return ans;
}
