/*
 * A251278  Number of (n+1)X(3+1) 0..3 arrays with no 2X2 subblock having its
 *          maximum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A251278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence (empirical) a(n) = 230*a(n-1) -9371*a(n-2)
 *            +68562*a(n-3) -428619*a(n-4) -3386398*a(n-5) +50923111*a(n-6)
 *            -44328410*a(n-7) -221985864*a(n-8) -608157648*a(n-9) (n ≥ 10)
 *            initial terms (n=1..9) are obtained from the combinatorial
 *            definition
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Helper constants for the recurrence */
static const int64_t coeffs[9] = {
   230LL,  -9371LL,  68562LL,  -428619LL, -3386398LL,
  50923111LL, -44328410LL, -221985864LL, -608157648LL
};

/* Pre‑computed data (filled once) */
static bool init_done = false;
static uint8_t row_digits[256][4];          /* base‑4 digits of a row pattern */
static uint16_t adj_cnt[256];
static uint16_t adj_lst[256][256];         /* adjacency list                 */
static seq_t first_terms[10];              /* first_terms[1..9]              */

/* Fill the digit table for all 256 row patterns */
static void init_digits(void) {
    for (int pat = 0; pat < 256; ++pat) {
        for (int c = 0; c < 4; ++c)
            row_digits[pat][c] = (pat >> (2 * c)) & 3;
    }
}

/* Compatibility test for two rows */
static bool compatible(unsigned r, unsigned s) {
    for (int j = 0; j < 3; ++j) {
        int a = row_digits[r][j];     /* top‑left   */
        int b = row_digits[r][j + 1]; /* top‑right  */
        int c = row_digits[s][j];     /* bottom‑left*/
        int d = row_digits[s][j + 1]; /* bottom‑right*/
        int maxdiag = a > d ? a : d;
        int minanti = b < c ? b : c;
        if (maxdiag < minanti)
            return false;
    }
    return true;
}

/* Build the adjacency list (256x256) */
static void init_adj(void) {
    for (int r = 0; r < 256; ++r) {
        adj_cnt[r] = 0;
        for (int s = 0; s < 256; ++s) {
            if (compatible((unsigned)r, (unsigned)s))
                adj_lst[r][adj_cnt[r]++] = (uint16_t)s;
        }
    }
}

/* Compute the first nine terms by a straightforward DP */
static void compute_first_terms(void) {
    /* DP vectors */
    static seq_t cur[256];
    static seq_t nxt[256];

    /* start with one array consisting of a single row: all rows are allowed */
    for (int i = 0; i < 256; ++i)
        cur[i] = (seq_t)1;

    /* compute a(1) up to a(9) */
    for (int step = 1; step <= 9; ++step) {
        /* zero next vector */
        for (int i = 0; i < 256; ++i)
            nxt[i] = (seq_t)0;

        /* propagate one row */
        for (int r = 0; r < 256; ++r) {
            seq_t val = cur[r];
            if (val == (seq_t)0) continue;
            uint16_t c = adj_cnt[r];
            uint16_t *lst = adj_lst[r];
            for (uint16_t k = 0; k < c; ++k)
                nxt[lst[k]] += val;
        }

        /* switch vectors */
        for (int i = 0; i < 256; ++i) {
            cur[i] = nxt[i];
        }

        /* sum all states to get a(step) */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < 256; ++i)
            sum += cur[i];
        first_terms[step] = sum;
    }
}

/* One‑time initialization routine */
static void init(void) {
    if (!init_done) {
        init_digits();
        init_adj();
        compute_first_terms();
        init_done = true;
    }
}

/* Main function to compute a(n) */
seq_t a(idx_t n) {
    init();                     /* ensure everything is ready */

    if (n <= 9)                  /* small n – we have the exact value */
        return first_terms[(int)n];

    /* For larger n use the empirical 9‑term recurrence */
    /* ring[0] = a(current‑1), ring[1] = a(current‑2), … , ring[8] = a(current‑9) */
    seq_t ring[9];
    for (int i = 0; i < 9; ++i)
        ring[i] = first_terms[9 - i];   /* ring[0] = a(9) … ring[8] = a(1) */

    for (idx_t cur = 10; cur <= n; ++cur) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (int i = 0; i < 9; ++i) {
            seq_t prod = ring[i] * (seq_t)(coeffs[i] >= 0 ? coeffs[i] : -coeffs[i]);
            if (coeffs[i] >= 0)
                pos += prod;
            else
                neg += prod;
        }
        seq_t next = pos;
        next -= neg;                /* pos ≥ neg, result is non‑negative */

        /* rotate ring: drop a(cur‑9), shift and insert new value at front */
        for (int i = 8; i > 0; --i)
            ring[i] = ring[i - 1];
        ring[0] = next;
    }
    return ring[0];                /* a(n) */
}
