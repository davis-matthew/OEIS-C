/*
 * A251906  Number of (n+2)X(1+2) 0..3 arrays with no row, column, diagonal or
 *          antidiagonal in any 3X3 subblock summing to 4 or 5.
 * https://oeis.org/A251906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×3 matrices with entries 0..3 such that *
 *            in every 3×3 subblock no row, column, main diagonal or
 *            anti‑diagonal * sums to 4 or 5. This counts all arrays of size
 *            (n+2)×3 satisfying that * local restriction. The algorithm is a
 *            linear‑time dynamic programming * over triples of c...
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* EXTERNAL TYPES:
 *   seq_t – arbitrary precision integer type used by the build
 *   idx_t – unsigned integer type (uint64_t)                         */

/* ------------------------------------------------------------------ */
/*  Global data structures – built once by init_tables()               */
static bool      row_ok[64];                 /* row sum ≠ 4 or 5 */
static uint8_t   *adj_buf;                  /* flat list of next rows */
static uint32_t  *adj_start;                /* start index in adj_buf per pair */
static uint8_t   *adj_cnt;                  /* count of next rows per pair */
static size_t     adj_capacity;             /* allocated capacity of adj_buf */
static size_t     STATES;                   /* 64*64 = 4096 */
static seq_t    *dp_cur;                    /* DP for length L-1 */
static seq_t    *dp_nxt;                    /* DP for length L   */
static seq_t    *len_res;                   /* cache of results indexed by length L=n+2 */
static idx_t     len_cap;                   /* capacity of len_res */
static idx_t     max_len;                   /* highest length precomputed */
static bool      tables_inited = false;

/* ------------------------------------------------------------------ */
/*  Helper to test triple (a,b,c) for validity                       */
static bool triple_ok(uint8_t a, uint8_t b, uint8_t c)
{
    /* rows must individually avoid sums 4 or 5 */
    if (!row_ok[a] || !row_ok[b] || !row_ok[c]) return false;

    /* column sums */
    for (int j = 0; j < 3; ++j) {
        uint8_t s = (a >> (2 * j)) & 3;
        s += (b >> (2 * j)) & 3;
        s += (c >> (2 * j)) & 3;
        if (s == 4 || s == 5) return false;
    }

    /* main diagonal */
    uint8_t s = (a & 3)         + ((b >> 2) & 3) + ((c >> 4) & 3);
    if (s == 4 || s == 5) return false;

    /* anti‑diagonal */
    s = ((a >> 4) & 3) + ((b >> 2) & 3) + (c & 3);
    if (s == 4 || s == 5) return false;

    return true;
}

/* ------------------------------------------------------------------ */
/*  Initialize global tables once                               */
static void init_tables(void)
{
    if (tables_inited) return;
    tables_inited = true;

    STATES = 64 * 64;

    /* Precompute row_ok table */
    for (uint32_t r = 0; r < 64; ++r) {
        uint8_t sum =
            (r & 3) + ((r >> 2) & 3) + ((r >> 4) & 3);
        row_ok[r] = (sum != 4 && sum != 5);
    }

    /* First pass – count transitions per pair to allocate buffer */
    adj_cnt = (uint8_t *)calloc(STATES, sizeof(uint8_t));
    size_t total = 0;
    for (uint32_t a = 0; a < 64; ++a) {
        for (uint32_t b = 0; b < 64; ++b) {
            uint32_t idx = (a << 6) | b;          /* 64*a + b */
            if (!row_ok[a] || !row_ok[b]) {
                adj_cnt[idx] = 0;
                continue;
            }
            uint8_t cnt = 0;
            for (uint32_t c = 0; c < 64; ++c) {
                if (triple_ok((uint8_t)a, (uint8_t)b, (uint8_t)c))
                    ++cnt;
            }
            adj_cnt[idx] = cnt;
            total += cnt;
        }
    }

    /* Allocate linear buffer and start indices */
    adj_start = (uint32_t *)malloc(STATES * sizeof(uint32_t));
    adj_buf   = (uint8_t  *)malloc(total  * sizeof(uint8_t));

    adj_capacity = total;
    size_t pos = 0;
    for (uint32_t a = 0; a < 64; ++a) {
        for (uint32_t b = 0; b < 64; ++b) {
            uint32_t idx = (a << 6) | b;
            adj_start[idx] = (uint32_t)pos;
            uint8_t cnt = adj_cnt[idx];
            for (uint32_t c = 0; c < 64 && cnt > 0; ++c) {
                if (triple_ok((uint8_t)a, (uint8_t)b, (uint8_t)c)) {
                    adj_buf[pos++] = (uint8_t)c;
                    --cnt;
                }
            }
        }
    }

    /* Allocate DP arrays */
    dp_cur = (seq_t *)calloc(STATES, sizeof(seq_t));
    dp_nxt = (seq_t *)calloc(STATES, sizeof(seq_t));

    /* Base for length 2 (two rows) – all pairs allowed */
    for (uint32_t i = 0; i < STATES; ++i) dp_cur[i] = (seq_t)1;

    /* Result cache for lengths up to 3 (need answer for n=1) */
    len_cap = 4;               /* indices 0..3 inclusive */
    len_res = (seq_t *)calloc(len_cap, sizeof(seq_t));
    /* len_res[0] and [1] unused; len_res[2] counts length-2 arrays (not used)
       len_res[3] will hold answer for n=1 (length 3). We compute it in ensure_len. */

    max_len = 2;   /* we have computed up to length 2 */
}

/* ------------------------------------------------------------------ */
/*  Ensure that answers are available up to length L (rows)           */
static void ensure_len(idx_t L)
{
    if (L <= max_len) return;
    init_tables();

    /* Grow result cache if needed */
    if (L >= len_cap) {
        idx_t newCap = len_cap;
        while (newCap <= L) newCap <<= 1;
        seq_t *tmp = (seq_t *)realloc(len_res, newCap * sizeof(seq_t));
        if (!tmp) exit(1);
        memset(tmp + len_cap, 0, (newCap - len_cap) * sizeof(seq_t));
        len_res = tmp;
        len_cap = newCap;
    }

    /* Extend DP from max_len+1 up to L */
    for (idx_t curLen = max_len + 1; curLen <= L; ++curLen) {
        /* Transition: dp_cur (length curLen-1) -> dp_nxt (length curLen) */
        for (uint32_t idx = 0; idx < STATES; ++idx) {
            seq_t val = dp_cur[idx];
            if (val == 0) continue;
            uint32_t b = idx & 63;
            uint32_t start = adj_start[idx];
            uint8_t cnt = adj_cnt[idx];
            for (uint8_t k = 0; k < cnt; ++k) {
                uint32_t c = adj_buf[start + k];
                uint32_t nextIdx = (b << 6) | c;
                dp_nxt[nextIdx] += val;
            }
        }

        /* Sum all dp_nxt to obtain the total number of arrays of this length */
        seq_t sum = 0;
        for (uint32_t i = 0; i < STATES; ++i) sum += dp_nxt[i];
        len_res[curLen] = sum;

        /* Prepare for next step: clear dp_cur, swap dp_cur and dp_nxt */
        memset(dp_cur, 0, STATES * sizeof(seq_t));
        seq_t *tmp = dp_cur; dp_cur = dp_nxt; dp_nxt = tmp;
    }

    max_len = L;
}

/* ------------------------------------------------------------------ */
/*  The function required by the build system                          */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined below offset */
    idx_t L = n + 2;              /* number of rows in the array */
    ensure_len(L);
    return len_res[L];
}
