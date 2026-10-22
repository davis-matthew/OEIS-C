/*
 * A378997  Dirichlet inverse of A057723.
 * https://oeis.org/A378997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(1) = 1; For n > 1: a(n) = -∑_{d|n, d<n} A057723(n/d) * a(d)
 *            where A057723(p^k) = p + p^2 + … + p^k (k ≥ 1) and is
 *            multiplicative
 * Range:     i8 n <= 128, i16 n <= 9295, i32 n <= 9295, i64 n <= 9295,
 *            i128 n <= 9295, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build a small sieve for smallest prime factors (enough for n ≤ 100000). */
static int _sieve_ready = 0;
static idx_t _spf[100001];

static void _build_sieve(void)
{
    if (_sieve_ready) return;
    const int N = 100000;
    for (int i = 0; i <= N; ++i) _spf[i] = 0;
    _spf[0] = 0; _spf[1] = 1;
    for (int i = 2; i <= N; ++i) {
        if (_spf[i] == 0) {
            _spf[i] = i;
            if ((idx_t)i * (idx_t)i <= (idx_t)N) {
                for (int j = i * i; j <= N; j += i)
                    if (_spf[j] == 0) _spf[j] = i;
            }
        }
    }
    _sieve_ready = 1;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    _build_sieve();

    idx_t tmp = n;
    seq_t result = (seq_t)1;

    /* Factorise n using the pre‑computed smallest prime factor array. */
    while (tmp > 1) {
        idx_t p = _spf[tmp];
        int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            exp++;
        }

        /* Compute a(p^exp) by the local Dirichlet inverse recurrence. */
        seq_t aVal[65];   /* aVal[k] = a(p^k), 0 ≤ k ≤ exp */
        seq_t fVal[65];   /* fVal[k] = p + p^2 + … + p^k (k ≥ 1) */
        aVal[0] = (seq_t)1;
        fVal[0] = (seq_t)0;
        seq_t pPow = (seq_t)1;

        for (int k = 1; k <= exp; ++k) {
            pPow = pPow * (seq_t)p;      /* p^k */
            fVal[k] = fVal[k - 1] + pPow;/* cumulative sum of powers */
        }

        for (int k = 1; k <= exp; ++k) {
            seq_t sum = (seq_t)0;
            for (int j = 1; j <= k; ++j) {
                sum = sum + fVal[j] * aVal[k - j];
            }
            aVal[k] = -sum;
        }

        seq_t ap = aVal[exp];
        result = result * ap;
    }

    return result;
}
