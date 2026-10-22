/*
 * A340429  Array T(n, k) is the number x such that frac(x*phi) +
 *          frac(n*phi)*frac(k*phi) = 1 where phi is the golden ratio A001622
 *          and frac(y) is the fractional part of y, read by antidiagonals.
 * https://oeis.org/A340429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term read diagonally from the array T(n,k)
 *            where T(n,k) = 3*n*k - n*h(k) - k*h(n) h(m) = ceil(m / φ²), φ =
 *            (1+√5)/2 h(m) is obtained from the Beatty sequence of ⌊kφ²⌋. The
 *            array A_{φ²} is generated via the identity φ² = φ + 1, giving
 *            ⌊kφ²⌋ = ⌊kφ⌋ + k. ⌊kφ⌋ is computed with intege...
 * Range:     u8 n <= 219, u16 n <= 9789, u32 n <= 9789, u64 n <= 9789,
 *            u128 n <= 9789, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const unsigned long long SCALE   = (unsigned long long)1 << 50;
/* scaled φ and (φ-1) (≈1/φ).  The values are chosen close enough that
   the rounding error is far below 1/(2*SCALE).  Accuracy to within a
   few units does not affect the final floor for the small indices
   required (≤ 200). */
static const unsigned long long PHI_SCALE = 1821706049271360ULL;   /* φ·2⁵⁰ (≈1.821 706 049 271 360) */
static const unsigned long long TAU_SCALE = PHI_SCALE - SCALE;     /* (φ-1)·2⁵⁰ (≈695 806 142 428 736) */

#define MAXK 200          /* enough for all indices needed up to n≈10 000 */

static bool initialized = false;
static unsigned long long a_seq[MAXK + 1];
static unsigned long long b_seq[MAXK + 1];

/* Initialize the Beatty sequences ⌊kφ⌋ (a_seq) and ⌊kφ²⌋ (b_seq). */
static void init_sequences(void)
{
    if (initialized) return;
    for (unsigned int k = 1; k <= MAXK; ++k) {
        /* ⌊kφ⌋ = k + ⌊k/φ⌋  */
        unsigned long long a = k + (( (unsigned long long)k * TAU_SCALE ) >> 50);
        a_seq[k] = a;
        /* ⌊kφ²⌋ = ⌊kφ⌋ + k  */
        b_seq[k] = a + k;
    }
    initialized = true;
}

/* Return h(m) = ceil(m / φ²) by the definition of the Beatty sequence:
   the smallest k with ⌊kφ²⌋ ≥ m.  The search range is 1 … MAXK. */
static seq_t get_h(idx_t m)
{
    init_sequences();
    int low = 1, high = MAXK;
    while (low < high) {
        int mid = (low + high) >> 1;
        if (b_seq[mid] >= m) high = mid;
        else low = mid + 1;
    }
    return (seq_t)low;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d-1)/2 >= n.  d starts at 2. */
    idx_t d = 2;
    while ((d * (d - 1)) / 2 < n) d++;

    /* Count of terms preceding this diagonal. */
    idx_t prev = ((d - 2) * (d - 1)) / 2;
    idx_t offset = n - prev - 1;      /* zero‑based position in diagonal */
    idx_t i = 1 + offset;             /* row index */
    idx_t j = d - i;                  /* column index */

    seq_t h_i = get_h(i);
    seq_t h_j = get_h(j);

    /* Compute T(i,j) = 3*i*j - i*h_j - j*h_i. */
    seq_t res = (seq_t)(3ULL * i * j) - (seq_t)i * h_j - (seq_t)j * h_i;
    return res;
}
