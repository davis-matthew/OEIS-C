/*
 * A251839  Number of (n+2) X (2+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum nonprime and every diagonal and antidiagonal sum
 *          prime.
 * https://oeis.org/A251839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 17*a(n-3) -
 *            90*a(n-6) + 191*a(n-9) - 205*a(n-12) * + 128*a(n-15) -
 *            48*a(n-18) + 8*a(n-21) for n > 26. * The first 26 terms are
 *            generated exactly by dynamic programming * over the last two
 *            rows of the (n+2)×4 array
 * Range:     u8 none, u16 n <= 9, u32 n <= 24, u64 n <= 54, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Preconditions: the build supplies idx_t (uint64_t) and seq_t
 * (unsigned integer or arbitrary‑precision class).  No further
 * typedefs are required. */

#define ROWS   256                 /* 4 cells with 2 bits each */
#define STATES 65536                /* state = (prev_row << 8) | cur_row */

/* Precomputed row decompositions */
static uint8_t row_val[ROWS][4];

/* Small prime lookup for values up to 9 */
static bool prime_small[10];

/* Adjacency lists for valid transitions */
static uint16_t **adj;      /* dest states */
static size_t   *adj_cnt;   /* number of dests per state */

/* ------------------------------------------------------------------ */
/* Helper utilities */
/* ------------------------------------------------------------------ */
static inline bool is_prime_small_val(int x)
{
    return x >= 0 && x < 10 && prime_small[x];
}

static void init_row_vals(void)
{
    for (int r = 0; r < ROWS; ++r) {
        unsigned int t = r;
        for (int c = 0; c < 4; ++c) {
            row_val[r][c] = t & 3;
            t >>= 2;
        }
    }
}

/* Check if rows a,b,c form a valid 3×3 window for both column positions */
static bool triple_valid(int a, int b, int c)
{
    const uint8_t *A = row_val[a];
    const uint8_t *B = row_val[b];
    const uint8_t *C = row_val[c];

    /* Row sums, column sums: must be non‑prime */
    int rs[4];
    int cs[4];
    int d0,d1;

    /* first three columns */
    rs[0] = A[0]+A[1]+A[2];
    rs[1] = B[0]+B[1]+B[2];
    rs[2] = C[0]+C[1]+C[2];
    for (int i = 0; i < 3; ++i)
        if (is_prime_small_val(rs[i])) return false;

    cs[0] = A[0]+B[0]+C[0];
    cs[1] = A[1]+B[1]+C[1];
    cs[2] = A[2]+B[2]+C[2];
    for (int i = 0; i < 3; ++i)
        if (is_prime_small_val(cs[i])) return false;

    d0 = A[0]+B[1]+C[2];
    d1 = A[2]+B[1]+C[0];
    if (!is_prime_small_val(d0) || !is_prime_small_val(d1)) return false;

    /* last three columns */
    rs[0] = A[1]+A[2]+A[3];
    rs[1] = B[1]+B[2]+B[3];
    rs[2] = C[1]+C[2]+C[3];
    for (int i = 0; i < 3; ++i)
        if (is_prime_small_val(rs[i])) return false;

    cs[0] = A[1]+B[1]+C[1];
    cs[1] = A[2]+B[2]+C[2];
    cs[2] = A[3]+B[3]+C[3];
    for (int i = 0; i < 3; ++i)
        if (is_prime_small_val(cs[i])) return false;

    d0 = A[1]+B[2]+C[3];
    d1 = A[3]+B[2]+C[1];
    if (!is_prime_small_val(d0) || !is_prime_small_val(d1)) return false;

    return true;
}

/* Build adjacency lists for DP transitions */
static void build_adjacency(void)
{
    adj_cnt = (size_t *)calloc(STATES, sizeof(size_t));

    /* Count outgoing edges */
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            for (int c = 0; c < ROWS; ++c) {
                if (triple_valid(a, b, c))
                    ++adj_cnt[(a << 8) | b];
            }
        }
    }

    adj = (uint16_t **)malloc(STATES * sizeof(uint16_t *));
    for (int s = 0; s < STATES; ++s)
        adj[s] = adj_cnt[s] ? (uint16_t *)malloc(adj_cnt[s] * sizeof(uint16_t)) : NULL;

    /* Fill adjacency lists */
    size_t *idx = (size_t *)calloc(STATES, sizeof(size_t));
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            for (int c = 0; c < ROWS; ++c) {
                if (triple_valid(a, b, c)) {
                    int src = (a << 8) | b;
                    int dst = (b << 8) | c;
                    adj[src][idx[src]++] = (uint16_t)dst;
                }
            }
        }
    }
    free(idx);
}

/* ------------------------------------------------------------------ */
/* Dynamic programming for the initial 26 terms */
/* ------------------------------------------------------------------ */
static seq_t *init_cache = NULL;   /* init_cache[0] = a(1) */
static idx_t init_len   = 0;

static void compute_initial_terms(void)
{
    if (init_len) return;           /* already computed */

    /* prime lookup for 0..9 */
    for (int i = 0; i < 10; ++i)
        prime_small[i] = (i == 2 || i == 3 || i == 5 || i == 7);

    init_row_vals();
    build_adjacency();

    init_cache = (seq_t *)calloc(26, sizeof(seq_t));
    init_len   = 26;

    seq_t *dp  = (seq_t *)calloc(STATES, sizeof(seq_t));
    seq_t *ndp = (seq_t *)calloc(STATES, sizeof(seq_t));

    /* All two‑row states are admissible before any rows are placed */
    for (int i = 0; i < STATES; ++i) dp[i] = (seq_t)1;

    for (idx_t step = 1; step <= 26; ++step) {
        /* zero ndp */
        for (int i = 0; i < STATES; ++i) ndp[i] = (seq_t)0;

        /* transition */
        for (int src = 0; src < STATES; ++src) {
            if (dp[src] == (seq_t)0) continue;
            seq_t val = dp[src];
            uint16_t *list = adj[src];
            size_t cnt = adj_cnt[src];
            for (size_t e = 0; e < cnt; ++e)
                ndp[list[e]] += val;
        }

        /* total arrays of height = step+2 */
        seq_t total = (seq_t)0;
        for (int i = 0; i < STATES; ++i)
            total += ndp[i];
        init_cache[step-1] = total;

        /* swap dp and ndp */
        seq_t *tmp = dp; dp = ndp; ndp = tmp;
    }

    free(dp);
    free(ndp);
}

/* ------------------------------------------------------------------ */
/* Recurrence cache for arbitrary n */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static const idx_t lag[7] = {3,6,9,12,15,18,21};
static const int   coeff[7] = {17,-90,191,-205,128,-48,8};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined, return 0 */

    compute_initial_terms();

    /* Expand cache as needed */
    if (cache_len < n) {
        if (cache_cap < n) {
            idx_t new_cap = n * 2;
            if (new_cap < 32) new_cap = 32;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        for (idx_t i = cache_len; i < n; ++i) {
            if (i < 26) {
                cache[i] = init_cache[i];
            } else {
                seq_t pos = (seq_t)0;
                seq_t neg = (seq_t)0;
                for (int k = 0; k < 7; ++k) {
                    idx_t m = i - lag[k];
                    seq_t term = cache[m];
                    if (coeff[k] > 0) pos += term * (seq_t)coeff[k];
                    else             neg += term * (seq_t)(-coeff[k]);
                }
                cache[i] = pos - neg;          /* pos >= neg for all n > 26 */
            }
        }
        cache_len = n;
    }

    return cache[n-1];
}
