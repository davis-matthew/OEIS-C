/*
 * A244178  Number of length n+4 0..3 arrays with no five consecutive elements
 *          with pattern ababa or abbba (possibly a=b) and new values 0..3
 *          introduced in 0..3 order.
 * https://oeis.org/A244178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A244178: Number of length n+4 0..3 arrays with no five consecutive
 * elements containing the pattern ababa or abbba, where new values 0..3
 * are introduced in 0..3 order.
 * Computation by dynamic programming over restricted growth strings
 * (RGFs) avoiding the two forbidden 5‑patterns.
 *
 * Recurrence:
 *   a(1) = 48
 *   a(2) = 173
 *   a(3) = 650
 *   a(4) = 2495
 *   a(5) = 9667
 *   ...
 *
 * DP state: (current maximum symbol m, last four symbols a,b,c,d) encoded
 * as an integer 0..1023.  All transitions preserve the RGF property
 * (new symbol may be ≤ m+1) and check the two forbidden patterns.
 *
 * Complexity:   O(1024·(n+4)) time,   O(1024) memory.
 * Works for any seq_t/idx_t types supplied by the build system.
 */

#include <stdbool.h>
#include <stdint.h>

static bool _init_done = false;
static unsigned char _last4_a[256];
static unsigned char _last4_b[256];
static unsigned char _last4_c[256];
static unsigned char _last4_d[256];
static uint16_t _next_state[1024][4];
static unsigned char _next_count[1024];

static void _init_transitions(void)
{
    if (_init_done) return;
    /* pre‑compute 4‑digit base‑4 decompositions for 0..255 */
    for (int i = 0; i < 256; ++i) {
        int d = i % 4;
        int c = (i / 4) % 4;
        int b = (i / 16) % 4;
        int a = (i / 64) % 4;
        _last4_a[i] = (unsigned char)a;
        _last4_b[i] = (unsigned char)b;
        _last4_c[i] = (unsigned char)c;
        _last4_d[i] = (unsigned char)d;
    }
    /* build successor table for each of the 1024 states */
    for (int state = 0; state < 1024; ++state) {
        unsigned int m  = state / 256;          /* current max (0..3) */
        unsigned int l4 = state % 256;          /* last 4 symbols */
        unsigned char a = _last4_a[l4];
        unsigned char b = _last4_b[l4];
        unsigned char c = _last4_c[l4];
        unsigned char d = _last4_d[l4];
        unsigned char cnt = 0;
        for (int v = 0; v <= m + 1 && v <= 3; ++v) {   /* next symbol */
            /* forbidden patterns: ababa or abbba */
            if ((a == c && b == d && a == v) ||
                (a == v && b == c && c == d))
                continue;
            unsigned int newmax = (v > m) ? v : m;
            unsigned int l4n = (((b * 4 + c) * 4 + d) * 4 + v);
            unsigned int newstate = newmax * 256 + l4n;
            _next_state[state][cnt] = (uint16_t)newstate;
            ++cnt;
        }
        _next_count[state] = cnt;
    }
    _init_done = true;
}

seq_t a(idx_t n)
{
    /* Length of arrays to count */
    idx_t L = n + 4;              /* n>=1 => L>=5 */

    _init_transitions();

    static seq_t dp[2][1024];
    seq_t *curr = dp[0];
    seq_t *next = dp[1];

    /* Zero DP for length 4 */
    for (idx_t i = 0; i < 1024; ++i) curr[i] = 0;

    /* Initialise from all length‑4 RGFs */
    for (int l4 = 0; l4 < 256; ++l4) {
        unsigned char a = _last4_a[l4];
        unsigned char b = _last4_b[l4];
        unsigned char c = _last4_c[l4];
        unsigned char d = _last4_d[l4];
        if (a != 0) continue;          /* first symbol must be 0 */
        unsigned int maxv = 0;
        if (b > maxv + 1) continue;    else maxv = (b > maxv) ? b : maxv;
        if (c > maxv + 1) continue;    else maxv = (c > maxv) ? c : maxv;
        if (d > maxv + 1) continue;    else maxv = (d > maxv) ? d : maxv;
        unsigned int state = maxv * 256 + (unsigned int)l4;
        curr[state] += (seq_t)1;
    }

    /* DP for positions 5 .. L */
    for (idx_t pos = 5; pos <= L; ++pos) {
        for (idx_t i = 0; i < 1024; ++i) next[i] = 0;
        for (int s = 0; s < 1024; ++s) {
            seq_t val = curr[s];
            if (!val) continue;
            unsigned char cnt = _next_count[s];
            for (unsigned char k = 0; k < cnt; ++k) {
                uint16_t ns = _next_state[s][k];
                next[ns] += val;
            }
        }
        /* swap */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    /* Sum remaining states */
    seq_t result = 0;
    for (idx_t i = 0; i < 1024; ++i) result += curr[i];
    return result;
}
