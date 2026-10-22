/*
 * A378963  Sequence of primitive Pythagorean triples beginning with the
 *          triple (3,4,5), with each subsequent triple having as its inradius
 *          the short leg of the previous triple, and with the long leg and
 *          the hypotenuse of each triple being consecutive natural numbers.
 * https://oeis.org/A378963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A378963
   Let t = floor((n-1)/3) + 1  (the index of the triple)
   let R = 2^t
   Then
     a(n) = 2*R - 1           if (n-1) % 3 == 0  (short leg)
     a(n) = 2*R*(R-1)         if (n-1) % 3 == 1  (long leg)
     a(n) = 2*R*(R-1) + 1     if (n-1) % 3 == 2  (hypotenuse)
   (i.e. the short leg, long leg, and hypotenuse of the
    primitive Pythagorean triple whose inradius is the
    short leg of the previous triple.) */

seq_t a(idx_t n)
{
    /* cache of powers of two: pow2[i] = 2^i  (i ≥ 0) */
    static seq_t pow2[4000];
    static idx_t pow2_len = 0;          /* number of entries computed */

    if (pow2_len == 0) {                /* initialise */
        pow2[0] = 1;
        pow2_len = 1;
    }

    /* index of the triple (t = 1 for first triple) */
    idx_t t = (n - 1) / 3 + 1;

    /* extend the cache if necessary */
    while (pow2_len <= t) {
        pow2[pow2_len] = pow2[pow2_len - 1] * 2;
        ++pow2_len;
    }

    seq_t R = pow2[t];              /* R = 2^t */
    seq_t Rm1 = R;
    Rm1 -= 1;                       /* R-1 */

    idx_t rem = (n - 1) % 3;

    if (rem == 0) {                  /* short leg */
        seq_t ans = R;
        ans *= 2;
        ans -= 1;
        return ans;
    } else if (rem == 1) {           /* long leg */
        seq_t ans = R;
        ans *= Rm1;
        ans *= 2;
        return ans;
    } else {                         /* hypotenuse */
        seq_t ans = R;
        ans *= Rm1;
        ans *= 2;
        ans += 1;
        return ans;
    }
}
