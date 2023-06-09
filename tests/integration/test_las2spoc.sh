# Fail on error
set -e

./build/debug/las2spoc --help 2> /dev/null

# Create a tmp directory for intermediate files
TMPDIR=$(mktemp --tmpdir --directory spoc.XXXXXXXX)

# Create a cleanup function
function cleanup {
    rm -rf ${TMPDIR}
}

# Run cleanup on exit
trap cleanup EXIT

./build/debug/las2spoc ./test_data/lidar/juarez50.las ${TMPDIR}/juarez50.spoc
./build/debug/las2spoc ./test_data/lidar/juarez50.las > ${TMPDIR}/juarez50.spoc
