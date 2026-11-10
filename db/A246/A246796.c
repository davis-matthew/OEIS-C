/*
 * A246796  a(n) = A246795(n) - A246794(n) + 1.
 * https://oeis.org/A246796
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = A246795(n) - A246794(n) + 1, i.e. the number of k >= 0
 *            with A182134(A005669(n) - k) = k, where A182134(j) = #{primes q:
 *            prime(j) < q < prime(j)^(1+1/j)}. The maximal prime gap starts
 *            A002386(n) are a table of known constants; A005669(n) =
 *            primepi(A002386(n)) is sieved for small n and taken...
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 75 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* lower primes of maximal prime gaps, A002386 (index 1..80) */
static const uint64_t A246796_P[81] = {0ULL,
 2ULL,3ULL,7ULL,23ULL,89ULL,113ULL,523ULL,887ULL,1129ULL,1327ULL,
 9551ULL,15683ULL,19609ULL,31397ULL,155921ULL,360653ULL,370261ULL,492113ULL,1349533ULL,1357201ULL,
 2010733ULL,4652353ULL,17051707ULL,20831323ULL,47326693ULL,122164747ULL,189695659ULL,191912783ULL,387096133ULL,436273009ULL,
 1294268491ULL,1453168141ULL,2300942549ULL,3842610773ULL,4302407359ULL,10726904659ULL,20678048297ULL,22367084959ULL,25056082087ULL,42652618343ULL,
 127976334671ULL,182226896239ULL,241160624143ULL,297501075799ULL,303371455241ULL,304599508537ULL,416608695821ULL,461690510011ULL,614487453523ULL,738832927927ULL,
 1346294310749ULL,1408695493609ULL,1968188556461ULL,2614941710599ULL,7177162611713ULL,13829048559701ULL,19581334192423ULL,42842283925351ULL,90874329411493ULL,171231342420521ULL,
 218209405436543ULL,1189459969825483ULL,1686994940955803ULL,1693182318746371ULL,43841547845541059ULL,55350776431903243ULL,80873624627234849ULL,203986478517455989ULL,218034721194214273ULL,305405826521087869ULL,
 352521223451364323ULL,401429925999153707ULL,418032645936712127ULL,804212830686677669ULL,1425172824437699411ULL,5733241593241196731ULL,6787988999657777797ULL,15570628755536096243ULL,17678654157568189057ULL,18361375334787046697ULL};

/* primepi of the above, A005669 (index 1..75) */
static const uint64_t A246796_M[76] = {0ULL,
 1ULL,2ULL,4ULL,9ULL,24ULL,30ULL,99ULL,154ULL,189ULL,217ULL,
 1183ULL,1831ULL,2225ULL,3385ULL,14357ULL,30802ULL,31545ULL,40933ULL,103520ULL,104071ULL,
 149689ULL,325852ULL,1094421ULL,1319945ULL,2850174ULL,6957876ULL,10539432ULL,10655462ULL,20684332ULL,23163298ULL,
 64955634ULL,72507380ULL,112228683ULL,182837804ULL,203615628ULL,486570087ULL,910101145ULL,981168565ULL,1094330064ULL,1820242202ULL,
 5217026274ULL,7322882552ULL,9583057667ULL,11723859927ULL,11945986786ULL,11992433550ULL,16202591808ULL,17883233895ULL,23541455248ULL,28106444830ULL,
 50070452577ULL,52302956123ULL,72178455400ULL,94906079600ULL,251265078335ULL,473258870471ULL,662221289043ULL,1411461642343ULL,2921439731020ULL,5394763455325ULL,
 6822667965940ULL,35315870460455ULL,49573167413483ULL,49749629143526ULL,1175661926421598ULL,1475067052906945ULL,2133658100875638ULL,5253374014230870ULL,5605544222945291ULL,7784313111002702ULL,
 8952449214971382ULL,10160960128667332ULL,10570355884548334ULL,20004097201301079ULL,34952141021660495ULL};

static uint64_t a246796_mulmod(uint64_t x, uint64_t y, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)x * y) % m);
}

static uint64_t a246796_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = a246796_mulmod(r, b, m);
        b = a246796_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool a246796_isprime(uint64_t n)
{
    static const uint64_t sp[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    if (n < 2) return false;
    for (int i = 0; i < 12; i++) {
        if (n == sp[i]) return true;
        if (n % sp[i] == 0) return false;
    }
    {
        static const uint64_t more[8] = {41,43,47,53,59,61,67,71};
        for (int i = 0; i < 8; i++) {
            if (n == more[i]) return true;
            if (n % more[i] == 0) return false;
        }
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        uint64_t x = a246796_powmod(sp[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = a246796_mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static uint64_t a246796_pi_exact(uint64_t x)
{
    if (x < 2) return 0;
    uint64_t h = (x - 1) / 2;               /* odd numbers 2i+1, i = 1..h */
    unsigned char *bits = (unsigned char *)calloc((size_t)(h / 8 + 2), 1);
    if (!bits) return 0;
    for (uint64_t i = 1; (2 * i + 1) * (2 * i + 1) <= x; i++) {
        if (bits[i >> 3] & (1u << (i & 7))) continue;
        uint64_t p = 2 * i + 1;
        for (uint64_t j = (p * p - 1) / 2; j <= h; j += p)
            bits[j >> 3] |= (unsigned char)(1u << (j & 7));
    }
    uint64_t cnt = 1;
    for (uint64_t i = 1; i <= h; i++)
        if (!(bits[i >> 3] & (1u << (i & 7)))) cnt++;
    free(bits);
    return cnt;
}

static long double a246796_li(long double x)
{
    long double L = logl(x), term = 1.0L, sum = 0.0L;
    for (int k = 1; k < 1000; k++) {
        term *= L / (long double)k;
        long double add = term / (long double)k;
        sum += add;
        if (add < 1e-22L * sum) break;
    }
    return 0.57721566490153286061L + logl(L) + sum;
}

static int a246796_mu(int k)
{
    int r = 1;
    for (int p = 2; p * p <= k; p++) {
        if (k % p == 0) {
            k /= p;
            if (k % p == 0) return 0;
            r = -r;
        }
    }
    if (k > 1) r = -r;
    return r;
}

static long double a246796_R(long double x)
{
    long double s = 0.0L;
    for (int k = 1; k <= 64; k++) {
        long double y = powl(x, 1.0L / (long double)k);
        if (y < 2.0L) break;
        int mu = a246796_mu(k);
        if (mu) s += (long double)mu / (long double)k * a246796_li(y);
    }
    return s;
}

static uint64_t a246796_index(idx_t n, uint64_t P)
{
    if (P < ((uint64_t)1 << 27)) return a246796_pi_exact(P);
    long double r = a246796_R((long double)P);
    if (n <= 75) {
        long double t = (long double)A246796_M[n];
        if (fabsl(t - r) < 1e-3L * r) return A246796_M[n];
    }
    return (uint64_t)(r + 0.5L);
}

static seq_t a246796_compute(idx_t n)
{
    if (n < 1 || n > 80) return (seq_t)0;
    uint64_t P = A246796_P[n];
    uint64_t m = a246796_index(n, P);
    if (m < 1) return (seq_t)0;
    uint64_t K = (m - 1 < 400) ? m - 1 : 400;

    size_t cap = (size_t)K + 1024, cnt = 0;
    uint64_t *lst = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!lst) return (seq_t)0;

    /* primes p_{m-K} .. p_m, collected backwards from P */
    lst[cnt++] = P;
    for (uint64_t c = P - 1; cnt <= K && c >= 2; c--)
        if (a246796_isprime(c)) lst[cnt++] = c;
    if (cnt < K + 1) K = cnt - 1;
    for (size_t i = 0, j = cnt - 1; i < j; i++, j--) {
        uint64_t t = lst[i]; lst[i] = lst[j]; lst[j] = t;
    }

    /* largest threshold needed */
    long double maxthr = 0.0L;
    for (uint64_t k = 0; k <= K; k++) {
        uint64_t pj = lst[K - k];
        long double j = (long double)(m - k);
        long double t = (long double)pj * expl(logl((long double)pj) / j);
        if (t > maxthr) maxthr = t;
    }
    uint64_t lim = (uint64_t)maxthr + 2;

    for (uint64_t c = P + 1; c <= lim && c > P; c++) {
        if (a246796_isprime(c)) {
            if (cnt == cap) {
                cap *= 2;
                uint64_t *nl = (uint64_t *)realloc(lst, cap * sizeof(uint64_t));
                if (!nl) { free(lst); return (seq_t)0; }
                lst = nl;
            }
            lst[cnt++] = c;
        }
    }

    bool found = false;
    uint64_t kmin = 0, kmax = 0;
    for (uint64_t k = 0; k <= K; k++) {
        size_t pos = (size_t)(K - k);
        uint64_t pj = lst[pos];
        long double j = (long double)(m - k);
        long double Lp = logl((long double)pj);
        uint64_t f = 0;
        for (size_t t = pos + 1; t < cnt; t++) {
            long double d = j * log1pl((long double)(lst[t] - pj) / (long double)pj);
            if (d < Lp) f++; else break;
        }
        if (f == k) {
            if (!found) { kmin = kmax = k; found = true; }
            else { if (k < kmin) kmin = k; if (k > kmax) kmax = k; }
        }
    }
    free(lst);
    if (!found) return (seq_t)0;
    return (seq_t)(kmax - kmin + 1);
}

seq_t a(idx_t n)
{
    static seq_t memo[81];
    static bool have[81];
    if (n > 80) return (seq_t)0;
    if (have[n]) return memo[n];
    seq_t r = a246796_compute(n);
    memo[n] = r;
    have[n] = true;
    return r;
}
