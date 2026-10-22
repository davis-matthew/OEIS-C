/*
 * A349983  a(n) is the largest k such A000792(k) <= n.
 * https://oeis.org/A349983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3 + ⌊log₃(n)⌋ + ⌊log₃(n/2)⌋ + ⌊log₃(n/4)⌋ for n > 3 a(1)
 *            = 1, a(2) = 2, a(3) = 3
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const idx_t POW3[] = {
    1ULL, 3ULL, 9ULL, 27ULL, 81ULL, 243ULL, 729ULL, 2187ULL, 6561ULL,
    19683ULL, 59049ULL, 177147ULL, 531441ULL, 1594323ULL, 4782969ULL,
    14348907ULL, 43046721ULL, 129140163ULL, 387420489ULL, 1162261467ULL,
    3486784401ULL, 10460353203ULL, 31381059609ULL, 94143178827ULL,
    282429536481ULL, 847288609443ULL, 2541865828329ULL, 7625597484987ULL,
    22876792454961ULL, 68630377364883ULL, 205891132094649ULL,
    617673396283947ULL, 1853020188851841ULL, 5559060566557443ULL,
    16677181699666569ULL, 50031545098999707ULL, 150094635296999121ULL,
    450283905890997363ULL, 1350851717672992089ULL,
    4052555153018976267ULL, 12157665459056928801ULL
};

static idx_t floor_log3(idx_t n)
{
    /* binary search for the greatest exponent e with 3^e ≤ n */
    if (n == 0) return 0;           /* never used for n > 3 */
    idx_t low = 0, high = 40;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (POW3[mid] <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 3;
    idx_t k1 = floor_log3(n);
    idx_t k2 = floor_log3(n / 2);
    idx_t k3 = floor_log3(n / 4);
    seq_t result = 3;
    result += (seq_t)k1;
    result += (seq_t)k2;
    result += (seq_t)k3;
    return result;
}
