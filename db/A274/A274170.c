/*
 * A274170  Christoffel words as binary numbers.
 * https://oeis.org/A274170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binary number obtained from the lower Christoffel word of
 *            * the coprime pair (a,b) where a+b is the smallest integer S
 *            such that the * cumulative count of φ(t) for 2 ≤ t ≤ S reaches
 *            n. The word consists of * a x-steps (horizontal moves, encoded
 *            as 1) and b y-steps (vertical moves, * encode...
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: greatest common divisor for idx_t. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Helper: Euler's totient function for idx_t. */
static idx_t phi(idx_t n)
{
    idx_t result = n;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

/* Main function required by the problem. */
seq_t a(idx_t n)
{
    /* Find the sum S such that cumulative φ(t) for 2 ≤ t ≤ S ≥ n. */
    idx_t S = 2;
    unsigned __int128 cum = 0;          /* cumulative count (64‑bit may overflow) */
    unsigned __int128 prev_cum = 0;      /* cumulative before current S */
    while (1) {
        unsigned __int128 ph = phi(S);
        prev_cum = cum;
        cum += ph;
        if (cum >= n) break;
        ++S;
    }

    /* k-th coprime integer in [1, S-1], counted from 1. */
    idx_t k = (idx_t)(n - prev_cum);
    idx_t a_val = 0;
    idx_t count = 0;
    for (idx_t i = 1; i < S; ++i) {
        if (gcd_idx(i, S) == 1) {
            ++count;
            if (count == k) {
                a_val = i;
                break;
            }
        }
    }
    idx_t b_val = S - a_val;

    /* Generate the binary word and its numeric value. */
    seq_t val = 0;
    idx_t x = 0, y = 0;            /* number of horizontal / vertical steps taken so far */
    for (idx_t step = 0; step < S; ++step) {
        /* Can we move up?  requires a_val*(y+1) <= b_val*x and y<b_val. */
        if (y < b_val && a_val * (y + 1) <= b_val * x) {
            ++y;
            val = (val << 1);      /* append 0 */
        } else {
            ++x;
            val = (val << 1) | 1; /* append 1 */
        }
    }
    return val;
}
