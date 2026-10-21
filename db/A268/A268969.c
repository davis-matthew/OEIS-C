/*
 * A268969  Number of n X 6 0..2 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two not
 *          more than once.
 * https://oeis.org/A268969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
 * a(n) = 50*a(n-1) - 805*a(n-2) + 4662*a(n-3)
 *        - 12150*a(n-4) + 14580*a(n-5) - 6561*a(n-6)
 * with a(1)=336, a(2)=11664, a(3)=339480, a(4)=9093528,
 * a(5)=231730344, a(6)=5712070032 */

static seq_t cache[10001 + 1];
static idx_t cache_len = 0;      /* highest index currently computed */

/* initialise the first six terms */
static void init_cache(void)
{
    if (cache_len == 0) {
        cache[1] = (seq_t)336;
        cache[2] = (seq_t)11664;
        cache[3] = (seq_t)339480;
        cache[4] = (seq_t)9093528;
        cache[5] = (seq_t)231730344;
        cache[6] = (seq_t)5712070032;
        cache_len = 6;
    }
}

seq_t a(idx_t n)
{
    init_cache();

    if (n <= cache_len)
        return cache[n];

    /* compute missing values up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] * (seq_t)50;
        seq_t tmp;

        tmp = cache[i - 2] * (seq_t)805;  val -= tmp;
        tmp = cache[i - 3] * (seq_t)4662; val += tmp;
        tmp = cache[i - 4] * (seq_t)12150; val -= tmp;
        tmp = cache[i - 5] * (seq_t)14580; val += tmp;
        tmp = cache[i - 6] * (seq_t)6561;  val -= tmp;

        cache[i] = val;
    }

    cache_len = n;
    return cache[n];
}
