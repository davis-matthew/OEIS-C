/*
 * A384709  a(n) = [n > 1 and A076479(n) = -Möbius(A067029(n))], where [.] is
 *          the Iverson bracket.
 * https://oeis.org/A384709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n > 1 and mu(rad(n)) = -mu(e)], e = exponent of least
 *            prime factor of n; computed by trial division
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 8839 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static int A384709_mu_small(unsigned long long e)
{
    int cnt = 0;
    for (unsigned long long p = 2; p * p <= e; p++) {
        if (e % p == 0) {
            e /= p;
            if (e % p == 0) return 0;
            cnt++;
        }
    }
    if (e > 1) cnt++;
    return (cnt & 1) ? -1 : 1;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    idx_t t = n;
    int cnt = 0;
    unsigned long long first_e = 0;

    for (idx_t p = 2; p <= t / p; p++) {
        if (t % p == 0) {
            unsigned long long e = 0;
            while (t % p == 0) {
                t /= p;
                e++;
            }
            if (cnt == 0) first_e = e;
            cnt++;
        }
    }
    if (t > 1) {
        if (cnt == 0) first_e = 1;
        cnt++;
    }

    int mu_rad = (cnt & 1) ? -1 : 1;
    int mu_e = A384709_mu_small(first_e);
    return (seq_t)(mu_rad == -mu_e ? 1 : 0);
}
