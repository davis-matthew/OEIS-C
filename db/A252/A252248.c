/*
 * A252248  Number of (n+2) X (2+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not equal to 0 3 5 6 or 7 and every 3 X 3 diagonal
 *          and antidiagonal sum equal to 0 3 5 6 or 7.
 * https://oeis.org/A252248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 5*a(n-3) - 4*a(n-4) - 8*a(n-6) + 4*a(n-7) +
 *            5*a(n-9) - a(n-10) - a(n-12) for n>16 Initial values (DP) for
 *            n=1..16 are computed by enumeration
 * Range:     u8 none, u16 n <= 22, u32 n <= 53, u64 n <= 111, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static unsigned char digits[256][4];
static bool is_rowcol_ok[10];
static bool is_diag_ok[10];
static unsigned char *trans_table = NULL;
static bool trans_table_built = false;

static void init_digit_tables(void)
{
    static bool built = false;
    if (built) return;
    for (int pat = 0; pat < 256; ++pat) {
        int tmp = pat;
        for (int i = 0; i < 4; ++i) {
            digits[pat][i] = tmp & 3;
            tmp >>= 2;
        }
    }
    for (int s = 0; s <= 9; ++s) {
        is_rowcol_ok[s] = (s != 0 && s != 3 && s != 5 && s != 6 && s != 7);
        is_diag_ok[s]   = (s == 0 || s == 3 || s == 5 || s == 6 || s == 7);
    }
    built = true;
}

static void init_trans_table(void)
{
    if (trans_table_built) return;
    init_digit_tables();

    size_t size = (size_t)256 * 256 * 256;   /* 16,777,216 bytes */
    trans_table = (unsigned char *)malloc(size);
    if (!trans_table) exit(1);
    memset(trans_table, 0, size);

    for (int a = 0; a < 256; ++a) {
        const unsigned char *da = digits[a];
        for (int b = 0; b < 256; ++b) {
            const unsigned char *db = digits[b];
            for (int c = 0; c < 256; ++c) {
                const unsigned char *dc = digits[c];
                bool ok = true;
                for (int j = 0; j < 2; ++j) {
                    /* Row sums for each of the three rows */
                    int rs_a = da[j] + da[j+1] + da[j+2];
                    int rs_b = db[j] + db[j+1] + db[j+2];
                    int rs_c = dc[j] + dc[j+1] + dc[j+2];
                    if (!is_rowcol_ok[rs_a] || !is_rowcol_ok[rs_b] || !is_rowcol_ok[rs_c]) {
                        ok = false; break;
                    }
                    /* Column sums for the 3×3 block */
                    for (int col = 0; col < 3; ++col) {
                        int cs = da[j+col] + db[j+col] + dc[j+col];
                        if (!is_rowcol_ok[cs]) { ok = false; break; }
                    }
                    if (!ok) break;
                    /* Diagonal sums */
                    int d1 = da[j] + db[j+1] + dc[j+2];
                    int d2 = da[j+2] + db[j+1] + dc[j];
                    if (!is_diag_ok[d1] || !is_diag_ok[d2]) { ok = false; break; }
                }
                if (ok) {
                    size_t idx = ((size_t)a << 16) | ((size_t)b << 8) | (size_t)c;
                    trans_table[idx] = 1;
                }
            }
        }
    }
    trans_table_built = true;
}

/* DP for n <= 16 */
static seq_t *dp = NULL;
static seq_t *dpnxt = NULL;
static bool dp_built = false;

static seq_t compute_dp(idx_t n)
{
    if (!dp_built) {
        dp = (seq_t *)malloc(65536 * sizeof(seq_t));
        dpnxt = (seq_t *)malloc(65536 * sizeof(seq_t));
        if (!dp || !dpnxt) exit(1);
        dp_built = true;
    }

    /* initialise all pairs with 1 */
    for (size_t i = 0; i < 65536; ++i) dp[i] = 1;

    init_trans_table();

    size_t steps = (size_t)n;        /* number of rows to add after the first two */
    for (size_t step = 0; step < steps; ++step) {
        /* clear dpnxt */
        memset(dpnxt, 0, 65536 * sizeof(seq_t));
        for (size_t idx = 0; idx < 65536; ++idx) {
            seq_t cur = dp[idx];
            if (cur == 0) continue;
            int a = (int)(idx >> 8);
            int b = (int)(idx & 0xFF);
            size_t base = ((size_t)a << 16) | ((size_t)b << 8);
            for (int c = 0; c < 256; ++c) {
                if (trans_table[base | (size_t)c]) {
                    size_t next = ((size_t)b << 8) | (size_t)c;
                    dpnxt[next] += cur;
                }
            }
        }
        /* swap dp and dpnxt */
        seq_t *tmp = dp; dp = dpnxt; dpnxt = tmp;
    }

    /* sum all states */
    seq_t total = 0;
    for (size_t i = 0; i < 65536; ++i) total += dp[i];
    return total;
}

/* Cached sequence storage */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;      /* allocated capacity minus one (indices 0..cap) */
static idx_t cache_used = 0;     /* highest index computed so far (>=0) */
static bool cache_init = false;

static void ensure_cache_capacity(idx_t req)
{
    if (req <= cache_cap) return;
    idx_t new_cap = req + 1024;          /* a bit more room */
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache) exit(1);
    /* zero the newly allocated part */
    memset(new_cache + cache_cap + 1, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not used, but defined for safety */

    if (!cache_init) {
        /* Allocate space for the first 16 terms plus placeholder 0 */
        cache = (seq_t *)calloc(17, sizeof(seq_t));
        if (!cache) exit(1);
        cache_cap = 16;
        /* Compute terms 1 … 16 by DP */
        for (idx_t i = 1; i <= 16; ++i)
            cache[i] = compute_dp(i);
        cache_used = 16;
        cache_init = true;
    }

    if (n <= cache_used) return cache[n];

    ensure_cache_capacity(n);

    for (idx_t k = cache_used + 1; k <= n; ++k) {
        seq_t val = cache[k-1];
        val += (cache[k-3] * (seq_t)5);
        val += (cache[k-7] * (seq_t)4);
        val += (cache[k-9] * (seq_t)5);

        seq_t sub = (cache[k-4] * (seq_t)4);
        sub += (cache[k-6] * (seq_t)8);
        sub += cache[k-10];
        sub += cache[k-12];

        cache[k] = val - sub;
    }
    cache_used = n;
    return cache[n];
}
