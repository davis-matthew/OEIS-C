/*
 * A371590  Irregular table T(n, k), n >= 0, k = 1..max(2, 2^n), read by rows;
 *          the n-th row lists the nonnegative numbers whose Zeckendorf-binary
 *          representation has n nonleading zeros.
 * https://oeis.org/A371590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A371590 (Irregular table T(n,k))
 * The function generates the row for a given number of nonleading
 * zeros and returns the corresponding term.
 *
 * It works for the “normal” integer builds (uint8_t … uint64_t)
 * and the C++17 arbitrary‑precision build.
 * The algorithm enumerates all Zeckendorf‑binary representations
 * having exactly r nonleading zeros, sorts them, and indexes
 * into the flattened sequence.  The complexity is O(2^r) per
 * row; for n ≤ 10000 the largest r is 13, so the routine is
 * fast and well below the time limits.
 */

#include <stdlib.h>

/* Comparison function for qsort that works for all seq_t types
 * (primitive unsigned ints and C++ unsigned __int128 or BigInt). */
static int seq_cmp(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Fibonacci numbers used by the Zeckendorf binary system.
 * fib[1] = 1, fib[2] = 2, fib[i] = fib[i-1] + fib[i-2] for i ≥ 3. */
static seq_t fib[65];
static int fib_ready = 0;

static void init_fib(void)
{
    if (fib_ready) return;
    fib[1] = 1;
    fib[2] = 2;
    for (int i = 3; i <= 64; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
    fib_ready = 1;
}

/* Recursive generation of all valid bit patterns for a fixed row.
 * pos           : current Fibonacci position (>=1).
 * prev_bit      : bit in the previous higher position (0 or 1).
 * zeros_left    : still needed zeros among positions < pos.
 * accum         : value of bits already fixed (including previous ones).
 * row, cnt      : output array and its current fill count. */
static void rec_gen(int pos, unsigned char prev_bit,
                    int zeros_left, seq_t accum,
                    seq_t *row, size_t *cnt)
{
    if (pos == 0) {                     /* finished all positions */
        if (zeros_left == 0) {          /* all required zeros used */
            row[(*cnt)++] = accum;
        }
        return;
    }

    /* Option 1: put 0 at this position (if a zero is still required) */
    if (zeros_left > 0) {
        rec_gen(pos - 1, 0, zeros_left - 1, accum, row, cnt);
    }

    /* Option 2: put 1 at this position (must not follow a 1) */
    if (prev_bit == 0) {
        rec_gen(pos - 1, 1, zeros_left, accum + fib[pos], row, cnt);
    }
}

/* Generate and cache the entire row for a given number of nonleading
 * zeros r.  The cached data is kept for subsequent calls. */
static seq_t *row_cache[65];
static size_t row_len_cache[65];

/* Generates the row for r (0 ≤ r).  Assumes r ≤ 64. */
static void build_row(int r)
{
    if (row_cache[r] != NULL) return;          /* already cached */

    if (r == 0) {                              /* trivial row */
        row_cache[0] = (seq_t *)malloc(2 * sizeof(seq_t));
        row_cache[0][0] = 0;
        row_cache[0][1] = 1;
        row_len_cache[0] = 2;
        return;
    }

    init_fib();

    size_t len = 1ULL << r;                    /* 2^r elements */
    seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
    size_t cnt = 0;

    /* Minimum and maximum possible lengths of the Zeckendorf binary
       representation for this row.  The first bit is always 1. */
    int L_min = r + 1;                 /* all following bits zeros */
    int L_max = 1 + 2 * r;             /* alternation 0/1 can occupy this many bits */

    for (int L = L_min; L <= L_max; ++L) {
        seq_t acc = fib[L];            /* first bit (highest position) */
        int zeros_needed = r - 1;      /* one zero is forced at position L-1 */

        rec_gen(L - 2, 0, zeros_needed, acc, row, &cnt);
    }

    /* The recursion may produce the numbers in arbitrary order; sort them. */
    qsort(row, len, sizeof(seq_t), seq_cmp);

    row_cache[r] = row;
    row_len_cache[r] = len;
}

/* Public entry function for the sequence. */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Compute row index r: r = floor(log2(n)) for n ≥ 2. */
    int r = 0;
    idx_t t = n;
    while (t > 1) {
        t >>= 1;
        ++r;
    }

    /* First index of this row. */
    idx_t start = (r == 0) ? 0 : ((idx_t)1 << r);

    idx_t k = n - start;          /* offset within the row */
    size_t index = (size_t)k;     /* convert to size_t for array index */

    build_row(r);

    /* Safety check: should never happen for valid inputs. */
    if (index >= row_len_cache[r]) return 0;

    return row_cache[r][index];
}
