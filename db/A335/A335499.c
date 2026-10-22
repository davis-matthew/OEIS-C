/*
 * A335499  1st Lucas-Wythoff array (w(n,k)), by antidiagonals; see Comments.
 * https://oeis.org/A335499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A335499: 1st Lucas‑Wythoff array in antidiagonal order.
   w(n,1)   = [n*φ]*L(1) + (n-1)*L(0)
   w(n,2)   = n + [φ*[φ*n]]
   w(n,k≥3) = [n*φ]*L(k-1) + (n-1)*L(k-2)
   The nth term a(n) is the element at the n‑th anti‑diagonal position. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isqrt_seq.h"

static seq_t *lucas = NULL;
static size_t lucas_len = 0;
static size_t lucas_cap = 0;

/* Ensure Lucas numbers up to index idx are available. */
static void ensure_lucas_index(size_t idx) {
    if (lucas_len > idx) return;
    if (lucas == NULL) {
        lucas_cap = 4;
        lucas = (seq_t *)malloc(lucas_cap * sizeof(seq_t));
        lucas[0] = (seq_t)2;     /* L(0) */
        lucas[1] = (seq_t)1;     /* L(1) */
        lucas_len = 2;
    }
    while (lucas_len <= idx) {
        if (lucas_len >= lucas_cap) {
            lucas_cap *= 2;
            lucas = (seq_t *)realloc(lucas, lucas_cap * sizeof(seq_t));
        }
        lucas[lucas_len] = lucas[lucas_len-1] + lucas[lucas_len-2];
        lucas_len++;
    }
}

/* floor( n * φ )  with φ = (1+√5)/2,  n is idx_t.  Operates entirely in seq_t. */
static inline seq_t floor_phi(idx_t n) {
    seq_t nn   = (seq_t)n;
    seq_t sq   = nn * nn;
    seq_t temp = (seq_t)5 * sq;
    seq_t s    = isqrt_seq(temp);
    return (nn + s) / (seq_t)2;
}

/* Return the n-th element of the 1st Lucas‑Wythoff array (a(n)). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset is 1; n≥1 in practice */

    /* Find the anti‑diagonal sum s such that (s-1)*s/2 < n ≤ s*(s-1)/2. */
    idx_t low = 2, high = 2;
    while ((unsigned __int128)high * (high - 1) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid - 1) / 2;
        if (tri >= n) high = mid;
        else          low  = mid + 1;
    }
    idx_t s = low;   /* i + j */

    unsigned __int128 prev_tri128 = (unsigned __int128)(s - 1) * (s - 2) / 2;
    idx_t          prev_tri   = (idx_t)prev_tri128;
    idx_t          off        = n - prev_tri;   /* 1‑based offset on this diagonal */
    idx_t          i = off;                    /* row index   (1‑based) */
    idx_t          j = s - off;                /* column index (1‑based) */

    seq_t result;
    seq_t phi_i = floor_phi(i);

    if (j == 1) {
        /* w(i,1) = floor(i*φ) + 2*(i-1) */
        result = phi_i + (seq_t)(i - 1) * (seq_t)2;
    } else if (j == 2) {
        /* w(i,2) = i + floor( φ * floor( φ*i ) ) */
        idx_t phi_i_idx = (idx_t)phi_i;          /* safe for intended n range */
        seq_t phi_phi_i = floor_phi(phi_i_idx);
        result = (seq_t)i + phi_phi_i;
    } else {
        /* w(i,j) = floor(i*φ)*L(j-1) + (i-1)*L(j-2) */
        ensure_lucas_index(j - 1);
        result = phi_i * lucas[j - 1] + (seq_t)(i - 1) * lucas[j - 2];
    }
    return result;
}
