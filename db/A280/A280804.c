/*
 * A280804  Number of nX2 0..1 arrays with no element equal to more than two
 *          of its horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly two elements, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A280804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) -30*a(n-2) +48*a(n-3) -45*a(n-4) +18*a(n-5) *
 *            +44*a(n-6) -60*a(n-7) +30*a(n-8) -2*a(n-9) -51*a(n-10) *
 *            +21*a(n-11) +13*a(n-12) -3*a(n-13) +18*a(n-14) +7*a(n-15) *
 *            -9*a(n-16) -6*a(n-17) -a(n-18) * Initial values: * a(1)=0,
 *            a(2)=1, a(3)=3, a(4)=18, a(5)=72, a(6)=284, a(7)=1047, * a(...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int COEFFS[18] = {
    9, -30, 48, -45, 18, 44, -60, 30, -2, -51,
    21, 13, -3, 18, 7, -9, -6, -1
};

static const seq_t INIT[18] = {
    0ULL, 1ULL, 3ULL, 18ULL, 72ULL, 284ULL, 1047ULL,
    3722ULL, 12816ULL, 43077ULL, 142046ULL, 461225ULL,
    1478635ULL, 4689871ULL, 14740063ULL, 45963889ULL,
    142346346ULL, 438168498ULL
};

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* sequence is defined for n ≥ 1 */
    if (n <= 18) return INIT[n-1];

    /* allocate array of size n to store intermediate values */
    seq_t *arr = (seq_t*)malloc(n * sizeof(seq_t));
    if (!arr) return 0;            /* allocation failed, return 0 as fallback */

    for (idx_t i = 0; i < 18; ++i) arr[i] = INIT[i];

    for (idx_t i = 18; i < n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int j = 0; j < 18; ++j) {
            seq_t val = arr[i - 1 - j];
            int c = COEFFS[j];
            if (c >= 0)
                pos += val * (seq_t)c;
            else
                neg += val * (seq_t)(-c);
        }
        arr[i] = pos - neg;        /* pos >= neg for valid sequence values */
    }

    seq_t result = arr[n-1];
    free(arr);
    return result;
}
