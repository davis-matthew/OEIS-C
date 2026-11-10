/*
 * A358314  Triangle T(n,k) read by rows where T(2m - 1,k) = (A051845(2m -
 *          1,k))/(2m - 1) and T(2m,k) = A051845(2m,k)/m for m > 0, k > 0.
 * https://oeis.org/A358314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = A051845(n,k)/n for odd n, T(n,k) = A051845(n,k)/(n/2)
 *            for even n, where A051845(n,k) is the k-th permutation of digits
 *            1..n (lexicographic order) read in base n+1. Row n has n! terms,
 *            except that the OEIS data has only 22 terms in row 4: its last
 *            two terms, 4312_5/2 = 291 and 4321_5/2 =...
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static idx_t A358314_fact[21];
static bool A358314_ready = false;

static void A358314_init(void)
{
    if (!A358314_ready) {
        A358314_fact[0] = 1;
        for (int i = 1; i <= 20; ++i)
            A358314_fact[i] = A358314_fact[i - 1] * (idx_t)i;
        A358314_ready = true;
    }
}

static idx_t A358314_rowlen(int r)
{
    if (r == 4) return 22;   /* OEIS row 4 leaves out its last two permutations */
    return A358314_fact[r];
}

seq_t a(idx_t n)
{
    A358314_init();

    int r = 1;
    idx_t cum = 0;
    while (r < 20 && cum + A358314_rowlen(r) < n) {
        cum += A358314_rowlen(r);
        ++r;
    }

    idx_t pos = n - cum - 1;
    bool used[21];
    for (int i = 0; i < 21; ++i) used[i] = false;

    seq_t value = 0;
    for (int i = r - 1; i >= 0; --i) {
        idx_t f = A358314_fact[i];
        idx_t idx = pos / f;
        pos %= f;

        int digit = 0;
        idx_t count = 0;
        for (int d = 1; d <= r; ++d) {
            if (!used[d]) {
                if (count == idx) {
                    digit = d;
                    used[d] = true;
                    break;
                }
                ++count;
            }
        }
        value = value * (seq_t)(r + 1) + (seq_t)digit;
    }

    int div = (r & 1) ? r : r / 2;
    return value / (seq_t)div;
}
