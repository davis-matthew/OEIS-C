/*
 * A399520  a(n) = n - A000149(A000195(n)), where A000149 = floor o exp,
 *          A000195 = floor o log.
 * https://oeis.org/A399520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - floor(exp(floor(log(n)))) (indices start at 1)
 * Range:     u8 n <= 658, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const idx_t _ExpFloor[] = {
    1ULL,   /* e^0          */
    2ULL,   /* e^1          */
    7ULL,   /* e^2          */
    20ULL,  /* e^3          */
    54ULL,  /* e^4          */
    148ULL, /* e^5          */
    403ULL, /* e^6          */
    1096ULL, /* e^7          */
    2980ULL, /* e^8          */
    8103ULL, /* e^9          */
    22026ULL, /* e^10         */
    59874ULL, /* e^11         */
    162754ULL, /* e^12        */
    442413ULL, /* e^13        */
    1202604ULL, /* e^14        */
    3269017ULL, /* e^15        */
    8886110ULL, /* e^16        */
    24154952ULL, /* e^17        */
    65659969ULL, /* e^18        */
    178482300ULL, /* e^19        */
    485165195ULL, /* e^20        */
    1318815737ULL, /* e^21       */
    3559122107ULL, /* e^22       */
    9615738467ULL, /* e^23       */
    26059937473ULL, /* e^24      */
    70827866330ULL, /* e^25      */
    192611684777ULL, /* e^26      */
    523613254375ULL, /* e^27      */
    1420769296956ULL, /* e^28     */
    3845965694424ULL, /* e^29     */
    10412853861435ULL, /* e^30    */
    28159878247734ULL, /* e^31    */
    76112315201391ULL, /* e^32    */
    205827851711727ULL, /* e^33   */
    556100829761651ULL, /* e^34   */
    1502828399290649ULL, /* e^35   */
    4054543812879772ULL, /* e^36   */
    10938637553412285ULL, /* e^37  */
    29506824276656674ULL, /* e^38 */
    79795017105998376ULL, /* e^39 */
    215743475157224615ULL, /* e^40 */
    583882183708305060ULL, /* e^41 */
    1582560437431587298ULL, /* e^42 */
    4283449939736476500ULL, /* e^43 */
    11582352808717343273ULL  /* e^44 */
};
static const size_t _ExpFloorCnt = sizeof _ExpFloor / sizeof *_ExpFloor;

seq_t a(idx_t n)
{
    if (n <= 1ULL) return 0;             /* a(1) = 0 */

    /* find first _ExpFloor[j] > n; j is in [1, _ExpFloorCnt] */
    size_t lo = 0, hi = _ExpFloorCnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (_ExpFloor[mid] > n) hi = mid;
        else lo = mid + 1;
    }

    size_t k = (lo > 0) ? lo - 1 : 0;     /* _ExpFloor[k] <= n */
    size_t m;
    if (k > 0 && _ExpFloor[k] == n) m = k - 1;
    else m = k;                           /* floor(log n) */

    idx_t bound = _ExpFloor[m];
    idx_t result = n - bound;             /* n and bound fit in idx_t */
    return (seq_t)result;
}
